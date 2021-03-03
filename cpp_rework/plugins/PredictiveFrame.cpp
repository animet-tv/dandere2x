
/*
    This file is part of the Dandere2x project.

    Dandere2x is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Dandere2x is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
*/

/* 
========= Copyright aka_katto 2018, All rights reserved. ============
Original Author: aka_katto 
Date: 4/11/20 
Purpose: todo
 
===================================================================== */


#include "PredictiveFrame.h"

// remove
#include "../evaluator/MSE_Function.h"
#include "../evaluator/SSIM_Function.h"

//-----------------------------------------------------------------------------
// Purpose: Match every block using the chosen block_matching algorithm chosen
//          and the chosen evaluation function. This code is intended to be
//          done on multiple cores, hence should work in parallel.
//-----------------------------------------------------------------------------
void PredictiveFrame::parallel_function_call(int x, int y) {

    // check if stationary block match works.
    if (eval->evaluate(*this->current_frame,*this->next_frame,
                       *this->next_frame_compressed, x, y, x, y, block_size)) {
        //cout << "matched stationary" << endl;
        this->matched_blocks.emplace_back(x, y, x, y, -1);
    } else {
        Block matched_block = this->block_matcher->match_block(x, y, block_size);

        if (eval->evaluate(*this->next_frame,*this->current_frame,
                           *this->next_frame_compressed,
                           matched_block.x_start, matched_block.y_start, matched_block.x_end, matched_block.y_end, block_size)) {
            //cout << "matched blocked" << endl;

            cout << SSIM_Function::compute_ssim(*this->next_frame,*this->current_frame,
                                               matched_block.x_start, matched_block.y_start, matched_block.x_end, matched_block.y_end, block_size) << endl;


            this->matched_blocks.emplace_back(matched_block.x_start, matched_block.y_start, matched_block.x_end, matched_block.y_end, -1);
        }
    }
}

//-----------------------------------------------------------------------------
// Purpose: todo
//-----------------------------------------------------------------------------
void PredictiveFrame::run() {
    for (int x = 0; x < current_frame->get_width() / block_size; x++) {
        for (int y = 0; y < current_frame->get_height() / block_size; y++) {
            parallel_function_call(x * block_size, y * block_size);
        }
    }

}

//-----------------------------------------------------------------------------
// Purpose: todo
//-----------------------------------------------------------------------------
void PredictiveFrame::write(const string &output_file) {

}

void PredictiveFrame::update_frame() {

}
