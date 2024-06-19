module stack_behaviour_normal(
    inout wire[3:0] IO_DATA, 
    input wire RESET, 
    input wire CLK, 
    input wire[1:0] COMMAND,
    input wire[2:0] INDEX
    ); 

    reg[2:0] current_cell;
    reg[2:0] current_cell_tmp;
    reg[3:0] memory_cell [4:0];
    reg[3:0] out_data;
    reg[3:0] in_data;
    reg out_en;
    integer i;

    assign IO_DATA = out_en & CLK ? out_data : 4'hz;

    initial begin
        current_cell = 0;
        out_en = 0;
        for (i = 0; i < 5; i+=1) begin
            memory_cell[i] = 0;
        end
    end

    always @(*) begin
        if (RESET == 1) begin
            for (i = 0; i < 5; i+=1) begin
                memory_cell[i] = 0;
            end
        end
    end

    always @(posedge CLK) begin
        case (COMMAND)
            2'b01: begin
                out_en = 0;
                memory_cell[current_cell] = IO_DATA;
                current_cell += 1;
                if (current_cell == 3'b101) begin
                    current_cell = 0;
                end
            end
            2'b10: begin
                out_en = 1;
                current_cell -= 1;
                if (current_cell == 3'b111) begin
                    current_cell = 4;
                end
                out_data = memory_cell[current_cell];
            end
            2'b11: begin
                out_en = 1;
                
                current_cell_tmp = current_cell;
                for (int i = 0; i < INDEX + 1; i += 1) begin
                    current_cell_tmp -= 1;
                    if (current_cell_tmp == 3'b111) begin
                        current_cell_tmp = 4;
                    end
                end
                out_data = memory_cell[current_cell_tmp];
            end
        endcase
    end
endmodule
