#ifndef __TTV_SSTA_H__
#define __TTV_SSTA_H__

int ttv_ssta_set_checkloadedstore(ttvfig_list *tvf);
ssta_entry_values *ttv_ssta_getvalues(ttvline_list *tvl);
int ttv_ssta_getsigcapa(ttvsig_list *tvs, float *capa);

#endif //__TTV_SSTA_H__

