#
# help.tcl --
#
# Tcl help command. (see TclX manual)
# 
#------------------------------------------------------------------------------
# Copyright 1992-1999 Karl Lehenbauer and Mark Diekhans.
#
# Permission to use, copy, modify, and distribute this software and its
# documentation for any purpose and without fee is hereby granted, provided
# that the above copyright notice appear in all copies.  Karl Lehenbauer and
# Mark Diekhans make no representations about the suitability of this
# software for any purpose.  It is provided "as is" without express or
# implied warranty.
#------------------------------------------------------------------------------
# The help facility is based on a hierarchical tree of subjects (directories)
# and help pages (files).  There is a virtual root to this tree. The root
# being the merger of all "help" directories found along the $auto_path
# variable.
#------------------------------------------------------------------------------
# $Id: help.tcl,v 1.2 2007/11/23 10:59:34 anthony Exp $
#------------------------------------------------------------------------------
#

#@package: TclX-help help helpcd helppwd apropos

namespace eval ::help {
    variable curSubject "/"
}

#------------------------------------------------------------------------------
# Help command.

proc help {{what {}}} {
    variable ::help::lineCnt 0

    # Special case "help help", so we can get it at any level.

    if {($what == "help") || ($what == "?")} {
        help::HelpOnHelp
        return
    }

    set pathList [help::ConvertPath $what]
    if {[file isfile [lindex $pathList 0]]} {
        help::DisplayPage [lindex $pathList 0]
        return
    }

    help::ListSubject $what $pathList subjects pages
    set relativeDir [help::RelativePath [lindex $pathList 0]]

    if {[llength $subjects] != 0} {
        help::Display "\nSubjects available in $relativeDir:"
        help::DisplayColumns $subjects
    }
    if {[llength $pages] != 0} {
        help::Display "\nHelp pages available in $relativeDir:"
        help::DisplayColumns $pages
    }
}


#------------------------------------------------------------------------------
# helpcd command.  The name of the new current directory is assembled from the
# current directory and the argument.

proc helpcd {{dir /}} {
    variable ::help::curSubject

    set pathName [lindex [help::ConvertPath $dir] 0]

    if {![file isdirectory $pathName]} {
        error "\"$dir\" is not a subject" [list TCLXHELP NOTSUBJECT $dir]
    }

    set ::help::curSubject [help::RelativePath $pathName]
    return
}

#------------------------------------------------------------------------------
# Helpcd main.

proc helppwd {} {
    variable ::help::curSubject
    puts "Current help subject: $::help::curSubject"
}

#------------------------------------------------------------------------------
# apropos command.  This search the 

proc apropos {regexp} {
    variable ::help::lineCnt 0
    variable ::help::curSubject

    set stop 0
    foreach dir [help::RootDirs] {
        foreach brief [glob -nocomplain $dir/*.brf] {
            set briefFH [open $brief]
            while {[gets $briefFH line] != -1} {
                if {[regexp -nocase $regexp $line] == 1} {
                    set path [lindex $line 0]
                    set desc [lrange $line 1 end]
                    if {![help::Display [format "%s - %s" $path $desc]]} {
                        set stop 1
                    }
                }
            }
            close $briefFH
            if {$stop} break
        }
        if {$stop} break
    }
}

##
## Private Helper Routines
##

#----------------------------------------------------------------------
# Return a list of help root directories.

proc ::help::RootDirs {} {
    global auto_path
    set roots {}
    foreach dir $auto_path {
	if {[file isdirectory $dir/help]} {
	    lappend roots $dir/help
	}
    }
    return $roots
}

#--------------------------------------------------------------------------
# Take a path name which might have "." and ".." elements and flatten them
# out.  Also removes trailing and adjacent "/", unless its the only
# character.

proc ::help::FlattenPath pathName {
    set newPath {}
    foreach element [split $pathName /] {
	if {"$element" == "." || ![string compare "" $element]} continue

	if {"$element" == ".."} {
	    if {[llength [join $newPath /]] == 0} {
		error "Help: name goes above subject directory root" {} \
		    [list TCLXHELP NAMEABOVEROOT $pathName]
	    }
	    set index [expr [llength $newPath]-1]
	    set newPath [lreplace $newPath $index $index] 
	    continue
	}
	lappend newPath $element
    }
    set newPath [join $newPath /]

    # Take care of the case where we started with something line "/" or "/."

    if {("$newPath" == "") && [string match "/*" $pathName]} {
	set newPath "/"
    }

    return $newPath
}

#--------------------------------------------------------------------------
# Given a pathName relative to the virtual help root, convert it to a list
# of real file paths.  A list is returned because the path could be "/",
# returning a list of all roots. The list is returned in the same order of
# the auto_path variable. If path does not start with a "/", it is take as
# relative to the current help subject.  Note: The root directory part of
# the name is not flattened.  This lets other commands pick out the part
# relative to the one of the root directories.

proc ::help::ConvertPath pathName {
    variable curSubject

    if {![string match "/*" $pathName]} {
	if {![string compare $curSubject "/"]} {
	    set pathName "/$pathName"
	} else {
	    set pathName "$curSubject/$pathName"
	}
    }
    set pathName [FlattenPath $pathName]

    # If the virtual root is specified, return a list of directories.

    if {$pathName == "/"} {
	return [RootDirs]
    }

    # Not the virtual root find the first match.

    foreach dir [RootDirs] {
	if {[file readable $dir/$pathName]} {
	    return [list $dir/$pathName]
	}
    }

    # Not found, try to find a file matching only the file tail,
    # for example if --> <helpDir>/tcl/control/if.

    set fileTail [file tail $pathName]
    foreach dir [RootDirs] {
	set fileName [exec find $dir -name $fileTail | head -1]
	if {$fileName != {}} {
	    return [list $fileName]
	}
    }

    error "\"$pathName\" does not exist" {} \
	[list TCLXHELP NOEXIST $pathName]
}

#--------------------------------------------------------------------------
# Return the virtual root relative name of the file given its absolute
# path.  The root part of the path should not have been flattened, as we
# would not be able to match it.

proc ::help::RelativePath pathName {
    foreach dir [RootDirs] {
	if {[string range $pathName 0 [expr [string length $dir] - 1]] == $dir} {
	    set name [string range $pathName [string length $dir] end]
	    if {$name == ""} {set name /}
	    return $name
	}
    }
    if {![info exists found]} {
	error "problem translating \"$pathName\"" {} [list TCLXHELP INTERROR]
    }
}

#--------------------------------------------------------------------------
# Given a list of path names to subjects generated by ConvertPath, return
# the contents of the subjects.  Two lists are returned, subjects under
# that subject and a list of pages under the subject.  Both lists are
# returned sorted.  This merges all the roots into a virtual root.
# pathName is the string that was passed to ConvertPath and is used for
# error reporting.  *.brk files are not returned.

proc ::help::ListSubject {pathName pathList subjectsVar pagesVar} {
    upvar $subjectsVar subjects $pagesVar pages

    set subjects {}
    set pages {}
    set foundDir 0
    foreach dir $pathList {
	if {![file isdirectory $dir] || ![string compare [file tail $dir] CVS]} continue
	set foundDir 1
	foreach file [glob -nocomplain $dir/*] {
	    if {[lsearch {.brf .orig .diff .rej} [file extension $file]] \
		    >= 0} continue
	    if [file isdirectory $file] {
		lappend subjects [file tail $file]/
	    } else {
		lappend pages [file tail $file]
	    }
	}
    }
    if {!$foundDir} {
	if {![string compare $pathName /]} {
	    global auto_path
	    error "no \"help\" directories found on auto_path ($auto_path)" {} \
		[list TCLXHELP NOHELPDIRS]
	} else {
	    error "\"$pathName\" is not a subject" {} \
		[list TCLXHELP NOTSUBJECT $pathName]
	}
    }
    set subjects [lsort $subjects]
    set pages [lsort $pages]
    return {}
}

#--------------------------------------------------------------------------
# Display a line of output, pausing waiting for input before displaying if
# the screen size has been reached.  Return 1 if output is to continue,
# return 0 if no more should be outputed, indicated by input other than
# return.
#

proc ::help::Display line {
    variable lineCnt
    if {$lineCnt >= 23} {
	set lineCnt 0
	puts -nonewline stdout ":"
	flush stdout
	gets stdin response
	if {[string compare "" $response]} {
	    return 0}
    }
    puts stdout $line
    incr lineCnt
}

#--------------------------------------------------------------------------
# Display a help page (file).

proc ::help::DisplayPage filePath {

    set inFH [open $filePath r]
	while {[gets $inFH fileBuf] >= 0} {
	    if {![Display $fileBuf]} {
		break
	    }
	}
	close $inFH
}

#--------------------------------------------------------------------------
# Display a list of file names in a column format. This use columns of 14 
# characters 3 blanks.

proc ::help::DisplayColumns {nameList} {
    set count 0
    set outLine ""
    foreach name $nameList {
	if {$count == 0} {
	    append outLine "   "
	}
	append outLine $name
	if {[incr count] < 4} {
	    set padLen [expr 17-[string length $name]]
	    if {$padLen < 3} {
		set padLen 3}
		set pad ""
		for {set i 0} {$i < $padLen} {incr i 1} {
		    append pad " "
		}
	    append outLine $pad
	} else {
	    if {![Display $outLine]} {
		return}
	    set outLine ""
	    set count 0
	}
    }
    if {$count != 0} {
	Display [string trimright $outLine]}
    return
}


#--------------------------------------------------------------------------
# Display help on help, the first occurance of a help page called "help" in
# the help root.

proc ::help::HelpOnHelp {} {
    set helpPage [lindex [ConvertPath /help] 0]
    if {![string compare "" $helpPage]} {
	error "No help page on help found" {} \
	    [list TCLXHELP NOHELPPAGE]
    }
    DisplayPage $helpPage
}

