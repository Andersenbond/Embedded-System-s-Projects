
 add_fsm_encoding \
       {wr_chnl.GEN_WDATA_SM_NO_ECC_DUAL_REG_WREADY.wr_data_sm_cs} \
       { }  \
       {{000 000} {001 100} {010 011} {011 001} {100 010} }

 add_fsm_encoding \
       {rd_chnl.rlast_sm_cs} \
       { }  \
       {{000 000} {001 010} {010 011} {011 100} {100 001} }

 add_fsm_encoding \
       {axi_lite.lite_sm_cs} \
       { }  \
       {{000 00} {001 01} {010 10} }

 add_fsm_encoding \
       {axi_data_fifo_v2_1_10_axic_reg_srl_fifo.state} \
       { }  \
       {{00 0100} {01 1000} {10 0001} {11 0010} }
