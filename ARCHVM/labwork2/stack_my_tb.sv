`include "stack_structural.sv"

module stack;
    reg[3:0] I_DATA; wire[3:0] O_DATA;
    reg RESET, CLK;
    reg[1:0] COMMAND; 
    wire[3:0] command;
    reg[2:0] INDEX;
    reg[2:0] pop_out, current_cell_;
    reg[3:0] data_i, o_data;
    wire q, n_q;
    wire[3:0] d [7:0];
    wire[7:0] s;

    // output wire[3:0] O_DATA, 
    // input wire RESET, 
    // input wire CLK, 
    // input wire[1:0] COMMAND, 
    // input wire[2:0] INDEX,
    // input wire[3:0] I_DATA
    

    substractor_3_3 stack_(pop_out, INDEX, current_cell_);

    always #1 CLK = ~CLK; 
    initial begin
        RESET = 1;
        $display("pop_out\to_data\tcurrent_cell");
        #3; RESET = 0;
    end

    initial begin      
        $dumpfile("dump.vcd"); $dumpvars(1, stack);
        CLK = 0;
        for (integer i = 0; i < 16; i += 1) begin
            #2; INDEX = i % 5; current_cell_ = i % 6; $display("%3b\t%3b\t%3b",pop_out, INDEX, current_cell_);
            
        end
        $finish;
    end

    // always @(*) begin
    //     $display("%3b\t%3b\t%3b",pop_out, INDEX, current_cell_);
    // end
endmodule