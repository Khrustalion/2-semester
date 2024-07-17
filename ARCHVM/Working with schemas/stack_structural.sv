
module stack_structural_lite(
    output wire[3:0] O_DATA, 
    input wire RESET, 
    input wire CLK, 
    input wire[1:0] COMMAND, 
    input wire[2:0] INDEX,
    input wire[3:0] I_DATA
    );
    wire[3:0] command;
    wire[2:0] current_cell_out;
    wire[2:0] current_cell_in;
    wire[2:0] push_out;
    wire[2:0] pop_out;
    wire[2:0] cmd0_out;
    wire[2:0] get_out;
    wire[3:0] pop_O_DATA;
    wire[3:0] get_O_DATA;
    wire[3:0] new_pop_O_DATA;
    wire[3:0] new_get_O_DATA;
    wire[7:0] s;
    wire[3:0] wire_memory_cell [7:0];
    wire[3:0] cell_[7:0];

    demultiplexer4 mult4(command, COMMAND);

    not (n_reset, RESET);

    or (new_CLK, CLK, RESET);

    or3 or_3(current_cell_in,  push_out, pop_out, cmd0_out, get_out);

    and14 pop_O_data(new_pop_O_DATA, new_CLK, pop_O_DATA);

    and14 get_o_data(new_get_O_DATA, new_CLK, get_O_DATA);

    or4_2wire or4_2(O_DATA, new_pop_O_DATA, new_get_O_DATA);
    
    always @(CLK) begin
        //$display(command[2], " ", current_cell_out, " ", pop_out, " ", cell_[0], " ", cell_[1], " ", cell_[2], " ", cell_[3], " ", cell_[4], " ", cell_[5], " ", cell_[6], " ", cell_[7]);
        //$display(CLK, " ", push_out, " ",  current_cell_out, " ", current_cell_in, " ", command[1]);
        //$display(CLK, " ", RESET, " ", current_cell_out, " ", cell_[0], " ", cell_[1], " ", cell_[2], " ", cell_[3], " ", cell_[4], " ", cell_[5], " ", cell_[6], " ", cell_[7]);
    end

    current_cell curr_cell(current_cell_out, current_cell_in, new_CLK, RESET);

    push push_(push_out, wire_memory_cell, s, command[1], I_DATA, current_cell_out);

    pop pop_(pop_out, pop_O_DATA, cell_, command[2], current_cell_out);

    command_00 cmd0(cmd0_out, current_cell_out, command[0]);

    get get_(get_out, get_O_DATA, INDEX, cell_, command[3], current_cell_out);

    wire[3:0] d0;
    wire[3:0] d1;
    wire[3:0] d2;
    wire[3:0] d3;
    wire[3:0] d4;
    wire[3:0] d5;
    wire[3:0] d6;
    wire[3:0] d7;

    and14 and_d0(d0, n_reset, wire_memory_cell[0]);
    and14 and_d1(d1, n_reset, wire_memory_cell[1]);
    and14 and_d2(d2, n_reset, wire_memory_cell[2]);
    and14 and_d3(d3, n_reset, wire_memory_cell[3]);
    and14 and_d4(d4, n_reset, wire_memory_cell[4]);
    and14 and_d5(d5, n_reset, wire_memory_cell[5]);
    and14 and_d6(d6, n_reset, wire_memory_cell[6]);
    and14 and_d7(d7, n_reset, wire_memory_cell[7]);

    or (s0, s[0], RESET);
    or (s1, s[1], RESET);
    or (s2, s[2], RESET);
    or (s3, s[3], RESET);
    or (s4, s[4], RESET);
    or (s5, s[5], RESET);
    or (s6, s[6], RESET);
    or (s7, s[7], RESET);

    memory_cell cell1(cell_[0], d0, new_CLK, s0);
    memory_cell cell2(cell_[1], d1, new_CLK, s1);
    memory_cell cell3(cell_[2], d2, new_CLK, s2);
    memory_cell cell4(cell_[3], d3, new_CLK, s3);
    memory_cell cell5(cell_[4], d4, new_CLK, s4);
    memory_cell cell6(cell_[5], d5, new_CLK, s5);
    memory_cell cell7(cell_[6], d6, new_CLK, s6);
    memory_cell cell8(cell_[7], d7, new_CLK, s7);



    // always @(*) begin
    //     $display(CLK, " ",COMMAND, " ", command[0], " ", command[1], " ", command[2], " ", command[3]);
    // end

    // reg[3:0] out_data;
    // wire[2:0] current_cell_out;
    // wire[2:0] current_cell_in;
    // wire[3:0] command;
    // wire[2:0] push_out;
    // wire[2:0] pop_out;


    // always begin
    //     #1; $display("%3b %2b %3b %3b", current_cell_out, pop_out, push_out, current_cell_in);
    // end
    //     //#4; $display("%5b\t%5b\t%5b\t%5b\t%5b\t%5b\t%5b\t%5b\t", cell_[0], cell_[1], cell_[2], cell_[3], cell_[4], cell_[5], cell_[6], cell_[7]);

    // put your code here, the other module MUST be deleted
    // don't edit module interface

endmodule

module or4_2wire(output wire[3:0] o_data, input wire[3:0] input_1, input_2);
    or (o_data[0], input_1[0], input_2[0]);
    or (o_data[1], input_1[1], input_2[1]);
    or (o_data[2], input_1[2], input_2[2]);
    or (o_data[3], input_1[3], input_2[3]);
endmodule

module command_00(output wire[2:0] data_o, input wire[2:0] data_i, input c0);
    and13 and1_3(data_o, c0, data_i);
endmodule

module or3(output wire[2:0] data_o, input wire[2:0] input_1, input_2, input_3, input_4);
    or (data_o[0], input_1[0], input_2[0], input_3[0], input_4[0]);
    or (data_o[1], input_1[1], input_2[1], input_3[1], input_4[1]);
    or (data_o[2], input_1[2], input_2[2], input_3[2], input_4[2]);
endmodule


module or4(output wire[3:0] data_o, input wire[3:0] input_1, input_2, input_3, input_4, input_5, input_6, input_7, input_8);
    or (data_o[0], input_1[0], input_2[0], input_3[0], input_4[0], input_5[0], input_6[0], input_7[0], input_8[0]);
    or (data_o[1], input_1[1], input_2[1], input_3[1], input_4[1], input_5[1], input_6[1], input_7[1], input_8[1]);
    or (data_o[2], input_1[2], input_2[2], input_3[2], input_4[2], input_5[2], input_6[2], input_7[2], input_8[2]);
    or (data_o[3], input_3[3], input_2[3], input_3[3], input_4[3], input_5[3], input_6[3], input_7[3], input_8[3]);
endmodule

module demultiplexer4(output wire[3:0] o, input wire[1:0] data_i);
    not (n_d0, data_i[0]);
    not (n_d1, data_i[1]);


    and (o[0], n_d0, n_d1);
    and (o[1], data_i[0], n_d1);
    and (o[2], n_d0, data_i[1]);
    and (o[3], data_i[0], data_i[1]);
endmodule

module rs_trigger(output wire q, n_q, input wire r, s, clk);
    and (and_r, clk, r);
    and (and_s, clk, s);
    wire nor_r, nor_s;
    nor (nor_r, and_r, nor_s);
    nor (nor_s, and_s, nor_r);
    assign q = nor_r;
    assign n_q = nor_s;
endmodule


module d_trigger(output q, input d, clk, reset);
    not (n_clk, clk);
    not (n_reset, reset);

    or (new_clk, clk, reset);
    or (new_n_clk, n_clk, reset);

    and (new_d, d, n_reset);
    not (n_d, new_d);

    rs_trigger rs_1(rs_1_q, rs_1_nq, n_d, new_d, new_clk);

    and (new_rs_1_q, rs_1_q, n_reset);
    or (new_rs_1_nq, rs_1_nq, reset);

    rs_trigger rs_2(q, rs_2_nq, new_rs_1_nq, new_rs_1_q, new_n_clk);
endmodule


module current_cell(output wire[2:0] data_o, input wire[2:0] data_i, input wire clk, reset);
    d_trigger d_1(data_o[0], data_i[0], clk, reset);
    d_trigger d_2(data_o[1], data_i[1], clk, reset);
    d_trigger d_3(data_o[2], data_i[2], clk, reset);
endmodule

module memory_cell(output wire[3:0] data_o, input wire[3:0] input_data, input wire clk, input wire state);
    wire tmp;
    rs_trigger bit1(data_o[0], tmp, ((~input_data[0]) & state), (input_data[0] & state), clk);
    rs_trigger bit2(data_o[1], tmp, ((~input_data[1]) & state), (input_data[1] & state), clk);
    rs_trigger bit3(data_o[2], tmp, ((~input_data[2]) & state), (input_data[2] & state), clk);
    rs_trigger bit4(data_o[3], tmp, ((~input_data[3]) & state), (input_data[3] & state), clk);
endmodule

module half_summer(output r, c, input a, b);
    and (and_a_b, a, b);
    xor (xor_a_b, a, b);
    assign r = xor_a_b;
    assign c = and_a_b;
endmodule

module summer_1_3(output wire[2:0] data_o, input wire a, input wire[2:0] b);
    half_summer sum0(data_o[0], carry_0, a, b[0]);
    half_summer sum1(data_o[1], carry_1, carry_0, b[1]);
    half_summer sum2(data_o[2], carry_2, carry_1, b[2]);
endmodule

module demultiplexer8(output wire[7:0] o, input wire[2:0] data_i);
    not (n_d0, data_i[0]);
    not (n_d1, data_i[1]);
    not (n_d2, data_i[2]);

    and (o[0], n_d0, n_d1, n_d2);
    and (o[1], data_i[0], n_d1, n_d2);
    and (o[2], n_d0, data_i[1], n_d2);
    and (o[3], data_i[0], data_i[1], n_d2);
    and (o[4], n_d0, n_d1, data_i[2]);
    and (o[5], data_i[0], n_d1, data_i[2]);
    and (o[6], n_d0, data_i[1], data_i[2]);
    and (o[7], data_i[0], data_i[1], data_i[2]);
endmodule

module and13(output wire[2:0] data_o, input d1, input wire[2:0] d3);
    and (data_o[0], d1, d3[0]);
    and (data_o[1], d1, d3[1]);
    and (data_o[2], d1, d3[2]);
endmodule


module and14(output wire[3:0] data_o, input d1, input wire[3:0] d4);
    and (data_o[0], d1, d4[0]);
    and (data_o[1], d1, d4[1]);
    and (data_o[2], d1, d4[2]);
    and (data_o[3], d1, d4[3]);
endmodule

module push(output wire[2:0] push_o, 
    output wire[3:0] d [7:0],
    output wire[7:0] s,

    input wire c1, 
    input wire[3:0] data_i,
    input wire[2:0] current_cell_
    );
    wire[7:0] o;
    wire[2:0] result_sum;

    summer_1_3 sum(result_sum, c1, current_cell_);

    and13 and_1_3(push_o, c1, result_sum);

    demultiplexer8 mult(o, result_sum);

    and14 d0(d[0], o[0], data_i);
    and (s[0], c1, o[0]);

    and14 d1(d[1], o[1], data_i);
    and (s[1], c1, o[1]);

    and14 d2(d[2], o[2], data_i);
    and (s[2], c1, o[2]);

    and14 d3(d[3], o[3], data_i);
    and (s[3], c1, o[3]);

    and14 d4(d[4], o[4], data_i);
    and (s[4], c1, o[4]);

    and14 d5(d[5], o[5], data_i);
    and (s[5], c1, o[5]);

    and14 d6(d[6], o[6], data_i);
    and (s[6], c1, o[6]);

    and14 d7(d[7], o[7], data_i);
    and (s[7], c1, o[7]);
endmodule

module half_substractor(output diff, bor, input a, b);
    xor (diff, a, b);

    not (n_a, a);
    and (bor, n_a, b);
endmodule

module substractor_3_1(output wire[2:0] data_o, input wire[2:0] data_i, input d);
    half_substractor h_sub1(data_o[0], bor1, data_i[0], d);
    half_substractor h_sub2(data_o[1], bor2, data_i[1], bor1);
    half_substractor h_sub3(data_o[2], bor3, data_i[2], bor2);
endmodule

module pop(output wire[2:0] pop_out,
    output wire[3:0] o_data,

    input wire[3:0] d [7:0],
    input wire c2, 
    input wire[2:0] current_cell_
    );
    wire[2:0] tmp_out;
    substractor_3_1 sub_3_1(tmp_out, current_cell_, c2);

    and13 and_13(pop_out, c2, tmp_out);

    wire[7:0] o;

    demultiplexer8 mult(o, current_cell_);

    wire[3:0] d_0;
    wire[3:0] d_1;
    wire[3:0] d_2;
    wire[3:0] d_3;
    wire[3:0] d_4;
    wire[3:0] d_5;
    wire[3:0] d_6;
    wire[3:0] d_7;

    and14 d0(d_0, o[0], d[0]);
    and14 d1(d_1, o[1], d[1]);
    and14 d2(d_2, o[2], d[2]);
    and14 d3(d_3, o[3], d[3]);
    and14 d4(d_4, o[4], d[4]);
    and14 d5(d_5, o[5], d[5]);
    and14 d6(d_6, o[6], d[6]);
    and14 d7(d_7, o[7], d[7]);

    wire[3:0] tmp_data;

    or4 or_4(tmp_data, d_0, d_1, d_2, d_3, d_4, d_5, d_6, d_7);

    and14 and_14(o_data, c2, tmp_data);
endmodule

module substractor(output d, bout, input a, b, bin);
    half_substractor hs1(hs1_diff, bor_1, a, b);
    half_substractor hs2(d, bor_2, hs1_diff, bin);

    or (bout, bor_1, bor_2);
endmodule

module substractor_3_3(output wire[2:0] data_o, input wire[2:0] input1, input2);
    half_substractor hs1(data_o[0], bor0, input1[0], input2[0]);
    substractor s2(data_o[1], bor1, input1[1], input2[1], bor0);
    substractor s3(data_o[2], bor2, input1[2], input2[2], bor1);
endmodule

module get(output wire[2:0] get_out, 
    output wire[3:0] o_data,

    input wire[2:0] index,
    input wire[3:0] d [7:0],
    input wire c3,
    input wire[2:0] current_cell_
    );

    and13 and1_3(get_out, c3, current_cell_);

    wire[2:0] tmp_out;

    substractor_3_3 sub_3_3(tmp_out, current_cell_, index);

    wire[7:0] o;

    demultiplexer8 mult(o, tmp_out);

    wire[3:0] d_0;
    wire[3:0] d_1;
    wire[3:0] d_2;
    wire[3:0] d_3;
    wire[3:0] d_4;
    wire[3:0] d_5;
    wire[3:0] d_6;
    wire[3:0] d_7;

    and14 d0(d_0, o[0], d[0]);
    and14 d1(d_1, o[1], d[1]);
    and14 d2(d_2, o[2], d[2]);
    and14 d3(d_3, o[3], d[3]);
    and14 d4(d_4, o[4], d[4]);
    and14 d5(d_5, o[5], d[5]);
    and14 d6(d_6, o[6], d[6]);
    and14 d7(d_7, o[7], d[7]);

    wire[3:0] tmp_data;

    or4 or_4(tmp_data, d_0, d_1, d_2, d_3, d_4, d_5, d_6, d_7);

    and14 and_14(o_data, c3, tmp_data);
endmodule
