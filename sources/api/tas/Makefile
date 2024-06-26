include $(AVERTEC_TOP)/etc/$(AVERTEC_OS).mk
include $(AVERTEC_TOP)/etc/libraries.mk
include $(AVERTEC_TOP)/etc/Avertec.mk

LOCAL_INC = -I$(TARGET_API_INCLUDE) -I$(TARGET_INCLUDE) 

LOCAL_CFLAGS   = $(CFLAGS) -c
LOCAL_CPPFLAGS = $(CPPFLAGS) $(LOCAL_INC) \
                 -DAVERTEC -D$(AVERTEC_LICENSE) -DTIMING_TOKEN\
                 -DAVT_H='"$(AVT_H)"' \
                 -DAPI_H='"$(API_H)"' \
                 -DMLU_H='"$(MLU_H)"' \
                 -DMLO_H='"$(MLO_H)"' \
                 -DTUT_H='"$(TUT_H)"' \
                 -DLIB_H='"$(LIB_H)"' \
                 -DSTB_H='"$(STB_H)"' \
                 -DTLC_H='"$(TLC_H)"' \
                 -DTLF_H='"$(TLF_H)"' \
                 -DELP_H='"$(ELP_H)"' \
                 -DLOG_H='"$(LOG_H)"' \
                 -DBVL_H='"$(BVL_H)"' \
                 -DBEH_H='"$(BEH_H)"' \
                 -DBEG_H='"$(BEG_H)"' \
                 -DBHL_H='"$(BHL_H)"' \
                 -DBEF_H='"$(BEF_H)"' \
                 -DMUT_H='"$(MUT_H)"' \
                 -DTRC_H='"$(TRC_H)"' \
                 -DMSL_H='"$(MSL_H)"' \
                 -DRCN_H='"$(RCN_H)"' \
                 -DCNS_H='"$(CNS_H)"' \
                 -DYAG_H='"$(YAG_H)"' \
                 -DINF_H='"$(INF_H)"' \
                 -DGEN_H='"$(GEN_H)"' \
                 -DTAS_H='"$(TAS_H)"' \
                 -DTRC_H='"$(TRC_H)"' \
                 -DSTM_H='"$(STM_H)"' \
                 -DTTV_H='"$(TTV_H)"' \
                 -DEQT_H='"$(EQT_H)"' \
                 -DMCC_H='"$(MCC_H)"' \
                 -DSIM_H='"$(SIM_H)"' \
                 -DGSP_H='"$(GSP_H)"' \
                 -DEFG_H='"$(EFG_H)"' \
                 -DCBH_H='"$(CBH_H)"' \
                 -DFCL_H='"$(FCL_H)"'

TCL_WRAPPER = tas_TCL_wrap.c
OBJ = tas_API.o

# Rules

# Static linking for Purify
$(TARGET_LIB)/$(APITAS_LIB): $(OBJ)
	$(AR) $(ARFLAGS) $(TARGET_LIB)/$(APITAS_LIB) $(OBJ)
	$(RANLIB) $(TARGET_LIB)/$(APITAS_LIB)
	cp tas_API.h $(TARGET_API_INCLUDE)

# Shared lib
dynamic_link: $(OBJ) 
	$(SCC) $(CFLAGS) -shared $(OBJ) tas_TCL_wrap.o -o $(TARGET_API_LIB)/tas_API.so
	cp tas_API.h $(TARGET_API_INCLUDE)

# TCL wrapper
$(TCL_WRAPPER): tas.i
	$(SWIG) -o $(TCL_WRAPPER) -tcl8 tas.i 

# Objects
.c.o :
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_CPPFLAGS) $<

clean :
	-@$(RM) -f $(WRAPPER) $(TCL_WRAPPER) $(OBJ) > /dev/null 2>&1

realclean : clean
	-@$(RM) -f $(TARGET_API_LIB)/tas_API.so $(TARGET_API_INCLUDE)/tas_API.h > /dev/null 2>&1
	-@$(RM) -f $(TARGET_LIB)/$(APITAS_LIB) > /dev/null 2>&1
