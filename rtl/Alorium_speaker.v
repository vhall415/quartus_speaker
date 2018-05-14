//code written by Ryan Milton to run speakers at 17 different frequencies
// part of our senior design project 4/4/2018
//Version 1

//im not sure this will effect the other modules
`timescale 1us/1ps

module Alorium_speaker
	(
	// assign inputs
	input wire clk,		//assume clock speed is 32 Mhz; 3.125*10^-8 = 31.25ns
    input wire resetn,  // reset when 0
	input wire spk_on, //determines when speaker is on
	
	output wire spk1_pin, //speaker signal which goes into a DAC
	output wire spk2_pin	
	); //<-- sad winking face
	
	reg [15:0] count1 = 0;
	reg [15:0] count2 = 0;
	//reg [15:0] target1 = 40000; //.0025s 400hz
	reg [15:0] target2 = 20000; // .00125s 800hz
	reg [15:0] target1 = 16000; // .0005s 1000hz
    
    // reg [num bits per number] name [items in array]
    //reg [11:0][0:1] freq = {12'd400, 12'd800}; // error; this verilog doesn't allow for multiple packed dimensions
    reg [11:0] freq1 = 12'd400;
    reg [11:0] freq2 = 12'd800;
		//assign frequency numbers
		//integer freq[1:2]; //in future, we will have 17 
			//freq[1] = 400;
			//freq[2] = 800;//449
//			freq[3] = 504;
//			freq[4] = 534;
//			freq[5] = 599;
//			freq[6] = 673;
// 		freq[7] = 755;
//			freq[8] = 800;
	
	reg spk1_temp = 0;
	assign spk1_pin = spk1_temp;
	reg spk2_temp = 0;
	assign spk2_pin = spk2_temp;
	
	
	//initialize counter
	always @(posedge clk)begin
        if (!resetn) begin
            spk1_temp <= 0;
        end
        else if (spk_on) begin
			if (target1 == count1) begin
				spk1_temp = ~spk1_temp;
				count1 = 0;
			end
			else begin
				count1 = count1 + 1;
			end
		end
		
		else begin	// if spk not on, count stays 0
			count1 = 0;
		end
	end 
   



		integer i = 0;	
	//initalize code for speaker 1	
	
	//always function will run as long as spk_on = 1
	
//	always @(spk_on) begin
//		if (target == count)
//			spk1_pin = ~spk1_pin;
//			count = 0;
//		end
//	end
	

	//initalize code for speaker 2
		always @(posedge clk)begin
            if(!resetn) begin
                spk2_temp <= 0;
            end
            else if (spk_on) begin
			    if (target2 == count2) begin
				    spk2_temp = 1;// ~spk2_temp;
				    count2 = 0;
			    end
			    else begin
				    count2 = count2 + 1;
			    end
		    end
		
		    else begin	// if spk not on, count stays 0
		    	count2 = 0;
		    end
		end 
endmodule 
