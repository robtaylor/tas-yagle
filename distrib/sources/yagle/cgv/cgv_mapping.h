char * cgv_mapping_table[][3] = 
  {
    { "i1x", "u-1_d+1", "1"},
    { "nd2", "u-2_u-1_d+1+2", "2"},
    { "nd3", "u-3_u-2_u-1_d+1+2+3", "3"},
    { "nd4", "u-4_u-3_u-2_u-1_d+1+2+3+4", "4"},
    { "nd5", "u-5_u-4_u-3_u-2_u-1_d+1+2+3+4+5", "5"},
    { "nd6", "u-6_u-5_u-4_u-3_u-2_u-1_d+1+2+3+4+5+6", "6"},
    { "nd7", "u-7_u-6_u-5_u-4_u-3_u-2_u-1_d+1+2+3+4+5+6+7", "7"},
    { "nd8", "u-8_u-7_u-6_u-5_u-4_u-3_u-2_u-1_d+1+2+3+4+5+6+7+8", "8"},
    { "nr2", "u-2-1_d+1_d+2", "2"},
    { "nr3", "u-3-2-1_d+1_d+2_d+3", "3"},
    { "nr4", "u-4-3-2-1_d+1_d+2_d+3_d+4", "4"},
    { "nr5", "u-5-4-3-2-1_d+1_d+2_d+3_d+4_d+5", "5"},
    { "nr6", "u-6-5-4-3-2-1_d+1_d+2_d+3_d+4_d+5_d+6", "6"},
    { "nr7", "u-7-6-5-4-3-2-1_d+1_d+2_d+3_d+4_d+5_d+6_d+7", "7"},
    { "nr8", "u-8-7-6-5-4-3-2-1_d+1_d+2_d+3_d+4_d+5_d+6_d+7_d+8", "8"},
    { "oai21", "u-3-1_u-2_d+1+2_d+2+3", "3"}, // ao22
    //{ "", "u-4-3_u-2-1_d+1+3_d+1+4_d+2+3_d+2+4", "4"}, // ao2o22
    //{ "aoi22", "u-4-3_u-4-2_u-3-1_u-2-1_d+1+4_d+2+3", "4"}, // noa2a22
    { "ao22", "u-4-1_u-3-2_d+1+2_d+3+4", "4"}, // nmx2
    { "ao22", "u-4-3_u-2-1_d+1+4_d+2+3", "4"}, // nmx2
    { "ao22", "u-4-3_u-4-2_u-3-1_u-2-1_d+1+4_d+2+3", "4"},       //xr2                 
  };

char * cgv_cellmapping_table[][3] = {
                              { "", "", ""},                 
                             };
