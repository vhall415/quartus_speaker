///////////////////////////////////////////////////////////////////
//=================================================================
//  Copyright (c) Alorium Technology 2016
//  ALL RIGHTS RESERVED
//  $Id:  $
//=================================================================
//
// File name:  : xlr8_wrap_template.v
// Author      : Steve Phillips
// Description : Template for wrapping user XB in glue logic needed
//               to interface cleanly with AVR core
//
// This template module provides an starting point for the user to
// enable thier module to communicate cleanly with the AVR core. The
// AVR core provides a relatively simple, standard interface which
// must be adhered to in order to interoperate cleanly.
//
// 
//=================================================================
///////////////////////////////////////////////////////////////////

module xlr8_speaker  // NOTE: Change the module name to match your design
  #(
    // Parameter definitions. The ADDR parameters will be defined when
    // this module is instantiated. These PARAMETERS are just
    // examples, you'll want to create your own as needed.
    parameter SPEAKER_CTRL_ADDR = 0,    // bit 0 for speakers on(1) or off(0)
    parameter SPEAKER_DATA_ADDR = 0,    // bit 0 for speaker1 on(1) or off(0)
                                        // bit 1 for speaker2 on(1) or off(0)
    parameter WIDTH = 8 // length of address
    )
   (
    // Input/Ouput definitions for the module. These are standard and
    // while other ports could be added, these are required.
    //  
    // Clock and Reset
    input        clk, //       Clock
    input        rstn, //      Reset (n = inverted = reset when 0)
    input        clken, //     Clock Enable
    // I/O 
    input [7:0]  dbus_in, //   Data Bus Input
    output [7:0] dbus_out, //  Data Bus Output
    output       io_out_en, // IO Output Enable
    output       spk1_out,
    output       spk2_out,
    // DM
    input [7:0]  ramadr, //    RAM Address
    input        ramre, //     RAM Read Enable
    input        ramwe, //     RAM Write Enable
    input        dm_sel //    DM Select
    );
   
   //======================================================================
   // Three example interfaces to the user module: ctrl, data_ex1, 
   // and data_ex2
   
   // The ctrl interface is an example of a R/W control register at a
   // configured address. The control register can be written or read
   // by the AVR. Specific bits or bit fields can be fed into the user
   // module to control its function.

   logic ctrl_sel;
   logic ctrl_we;
   logic ctrl_re;
   logic [WIDTH-1:0] ctrl_reg;

   // The data interface is a read only register that can be read
   // by the AVR core. The data value is read from the user module.
   
   logic data_sel;
   logic data_we;
   logic data_re;
   logic [WIDTH-1:0] data_reg;

   // End, interfaces
   //----------------------------------------------------------------------

   
   //======================================================================
   //  Control select
   //
   // For each register interface, do control select based on address
   assign ctrl_sel = dm_sel && (ramadr == SPEAKER_CTRL_ADDR);
   assign ctrl_we  = ctrl_sel && ramwe;
   assign ctrl_re  = ctrl_sel && ramre;
   
   assign data_sel = dm_sel && (ramadr == SPEAKER_DATA_ADDR);
   assign data_we  = data_sel && ramwe;
   assign data_re  = data_sel && ramre;

   // Mux the data and enable outputs
   assign dbus_out =  ({8{ ctrl_sel }} & ctrl_reg) |
                      ({8{ data_sel }} & data_reg );

   assign io_out_en = ctrl_re ||
                      data_re ||

   // End, Control Select
   //----------------------------------------------------------------------
   

   //======================================================================
   // Load write data from AVR core into registers
   //
   // For data written from the AVR core to the user module, you may
   // want to register the value here so that it is held for reference
   // until the net update in value

   // Load control register
   
   always @(posedge clk or negedge rstn) begin
      if (!rstn) begin
        ctrl_reg <= {WIDTH{1'b0}};
      end
      else if (clken && ctrl_we) begin
        ctrl_reg <= dbus_in[WIDTH-1:0];
      end
   end // always @ (posedge clk or negedge rstn)
   
   // End, Load write data
   //----------------------------------------------------------------------
   
   
   //======================================================================
   // Instantiate user module
   //
   // Below is an example instantiation of a simple user module. It
   // provides just enough I/O to demonstrate how the above logic is
   // connected.
   
   assign spk1_out = data_reg[0];
   assign spk2_out = data_reg[1];
   
   Alorium_speaker speak_inst (// Clock and Reset
                    .clk         (clk),
                    .resetn     (rstn),
                    // Inputs
                    .spk_on      (ctrl_reg[0]),
                    // Output
                    .spk1_pin   (data_reg[0]),
                    .spk2_pin   (data_reg[1])
                    );
   
   // End, Instantiate user module
   //----------------------------------------------------------------------
   
endmodule

