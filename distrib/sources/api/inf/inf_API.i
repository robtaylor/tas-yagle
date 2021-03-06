void  inf_SetFigureName                  (char *name);
void  inf_DumpRegistry_i                 (char *name);
void  inf_CheckRegistry_i                (char *name, int level, List *argv);
void  inf_AddFile                        (char *filename, char *figname);
void  inf_DefineSlew                     (char *name, TimeValue slope_rise, TimeValue slope_fall);
void  inf_DefineSlope                    (char *name, TimeValue slope_rise, TimeValue slope_fall);
void  inf_DefineDelay                    (char *origin, char *destination, TimeValue delay);
void  inf_DefineStop                     (char *name);
void  inf_DefineRename                   (char *origin, char *destination);
void  inf_DefineSensitive                (char *name);
void  inf_DefineSuppress                 (char *name);
void  inf_DefineInputs                   (char *name);
void  inf_DefineNotLatch                 (char *name);
void  inf_DefineKeepTristateBehaviour(char *name);
void  inf_DefineRC                       (char *name);
void  inf_DefineNORC                     (char *name);
void  inf_DefineAsynchron                (char *name);
void  inf_DefineInter                    (char *name);
void  inf_DefineBreak                    (char *name);
void inf_DefineStrictSetup(char *name);
void  inf_DefineCkPrech                  (char *name);
void  inf_DefineNoFalling                (char *name);
void  inf_DefineNoRising                 (char *name);
void  inf_SetPeriod                      (TimeValue value);
void  inf_SetSetupMargin                 (TimeValue value);
void  inf_SetHoldMargin                  (TimeValue value);
void  inf_DefineConstraint               (char *name, int value);
void  inf_DefineConditionedCommandStates (char *name, char *state);
void  inf_DefineClockPriority            (char *origin, char *destination);
void  inf_DefineDLatch                   (char *name);
void  inf_DefineNotDLatch                (char *name);
void  inf_DefinePrecharge                (char *name);
void  inf_DefineNotPrecharge             (char *name);
void  inf_DefineModelLatchLoop           (char *name);
void  inf_DefineRS                       (char *name, char *type);
void  inf_DefineCkLatch                  (char *name);
void  inf_DefineNotCkLatch               (char *name);
void  inf_DefineDirout                   (char *name, int level);
void  inf_DefineClock                    (List *argv);
void  inf_DefineBypass                   (char *name, char *where);
void  inf_DefineMutex                    (char *type, List *argv);
void  inf_DefineCrosstalkMutex           (char *type, List *argv);
void  inf_DefineDisable                  (char *origin, char *destination);
void  inf_CleanFigure                    ();
void  inf_DefineAsynchronousClockGroup   (char *domain, List *argv, TimeValue period);
void  inf_DefineEquivalentClockGroup     (char *domain, List *argv);
void  inf_DefinePower                    (char *name, double voltage);
void  inf_DefineTemperature              (double temp);
void  inf_DefineFlipFlop                 (char *name);
void  inf_Drive                          (char *outputname);
void  inf_ExportSections                 (char *outputname, char *section);
void  inf_DefineConnectorSwing           (char *name, double lowlevel, double highlevel);
void  inf_DefineStability                (List *argv);
void  inf_DefineFalsePath                (List *argv);
void  inf_DefinePathDelayMargin          (char *type, char *name, double factor, TimeValue delta, char *pathtype);
void  inf_DefineIgnore                   (char *type, List *argv);
void  inf_DefineConnectorDirections      (char *type, List *argv);
void  inf_DefineDoNotCross               (char *name);
void  inf_DefineTransparent(char *name);
void  inf_DefineSlopeRange               (char *name, List *argv, char *type);
void  inf_DefineCapacitanceRange         (char *name, List *argv, char *type);
void  inf_MarkSignal                     (char *name, char *marks);
void  inf_MarkTransistor                 (char *name, char *marks);
void  inf_DisableTimingArc               (char *input, char *output, char *direction);
void  inf_DefineCharacteristics          (char *type, List *argv);
void inf_DefineDirective_sub(List *argv);
void inf_DefineFalseSlack_sub(List *argv);
void inf_DefineSwitchingProbability(char *name, double val);
void inf_DefineMemsym(char *name0, char *name1);

/* SDC support */
void  infsdc_create_clock                (char **argv);
void  infsdc_set_input_transition        (char **argv);
void  infsdc_set_load                    (char **argv);
void  infsdc_create_generated_clock      (char **argv);
void  infsdc_set_clock_latency           (char **argv);
void  infsdc_set_input_delay             (char **argv); 
void  infsdc_set_output_delay            (char **argv); 
void  infsdc_set_case_analysis           (char **argv);
void  infsdc_set_false_path              (char **argv);
void  infsdc_set_disable_timing          (char **argv); 
void  infsdc_set_multicycle_path         (char **argv); 
void  infsdc_not_supported               (char  *command); 
void infsdc_set_clock_uncertainty (char **argv);
void infsdc_set_clock_groups (char **argv);
void infsdc_set_max_delay (char **argv);
void infsdc_set_min_delay (char **argv);

