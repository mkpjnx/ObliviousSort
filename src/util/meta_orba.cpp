#include "src/include/util/sorting.h"
#include "src/include/util/common.h"
#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace libUtil {
    void checkOverflow(size_t index, size_t max_index) {
        if (index >= max_index)
            throw std::invalid_argument( "Bucket Overflow Detected" );
    }
    
    /** ----- Bin Placement ----- **/
    void binPlace(std::vector<std::vector<int>>* out, std::vector<std::vector<std::string>>* labels, 
                  std::vector<int> Ai, std::vector<int> Aj, std::vector<std::string> Li, 
                  std::vector<std::string> Lj, int i) {
        int filler_element = -1;
        std::string filler_label = "filler";
        
        std::vector<int> A0(Ai.size(), filler_element); 
        std::vector<int> A1(Aj.size(), filler_element);
        std::vector<std::string> L0(Li.size(), filler_label);
        std::vector<std::string> L1(Lj.size(), filler_label);
        
        int m = 0; int n = 0;

        for (size_t k = 0; k < Ai.size(); k++) {
            if (Li[k] == filler_label) break;
            
            checkOverflow(m, Li.size());
            checkOverflow(n, Li.size());
            
            char bucket = Li[k][i];
            if (bucket == '0') {
                A0[m] = Ai[k]; L0[m] = Li[k]; m++;
            } else {
                A1[n] = Ai[k]; L1[n] = Li[k]; n++;
            }
        }
        
        for (size_t k = 0; k < Aj.size(); k++) {
            if (Lj[k] == filler_label) break;
            
            checkOverflow(m, Lj.size());
            checkOverflow(n, Lj.size());

            char bucket = Lj[k][i];
            if (bucket == '0') {
                A0[m] = Aj[k]; L0[m] = Lj[k]; m++;
            } else {
                A1[n] = Aj[k]; L1[n] = Lj[k]; n++;
            }
        }
        
        /** assign updated vectors to next layer in butterfly network **/
        (*out)[0] = A0; (*out)[1] = A1;
        (*labels)[0] = L0; (*labels)[1] = L1;
    }
    
    
    
    /*
     * ----- Meta-ORBA (gamma = 2) -----
     * Input: I     : int array of length n = beta * Z / 2
     *        beta  : number of output bins, assumed to be power of 2
     *        Z     : bucket bin size
     *        gamma : branching factor (assumed to be 2 in code)
     */
    std::vector<std::vector<int>> meta_orba(std::vector<int> I, int beta, int Z) {
        srand( time(0) );
        /** Generate random label of given length **/
        auto randchar = []() -> char {
            const char charset[] = "01";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };
        
        int filler_element = -1;
        std::string filler_label = "filler";
        
        /* Assign each input element with a random label of log(beta) bits */
        int label_len = log2(beta);
        std::vector<std::string> labels(I.size(), std::string(label_len, 0));
        for (size_t i = 0; i < I.size(); i++) {
            std::generate_n(labels[i].begin(), label_len, randchar);
        }
        
        std::vector<std::vector<int>> A_in(beta, std::vector<int>(Z, filler_element));
        std::vector<std::vector<std::string>> labels_in(beta, std::vector<std::string>(Z, filler_label));
        /* View I as list of beta bins, each with Z / 2 elements
         * Pad each bin with filler elements to capacity of Z */
        for (int i = 0; i < beta; i++) {
            std::copy(I.begin() + i * Z / 2, I.begin() + (i + 1) * Z / 2, A_in[i].begin());
            std::copy(labels.begin() + i * Z / 2, labels.begin() + (i + 1) * Z / 2, labels_in[i].begin());
        }
        
        std::vector<std::vector<int>> A_out(beta, std::vector<int>(Z, filler_element));
        std::vector<std::vector<std::string>> labels_out(beta, std::vector<std::string>(Z, filler_label));
        
        std::vector<std::vector<int>> temp(2, std::vector<int>(Z, filler_element));
        std::vector<std::vector<std::string>> labels_temp(2, std::vector<std::string>(Z, filler_label));
        /* Obliviously place elements into corresponding bins */
        for (int i = 0; i < log2(beta); i++) {
            for (int j = 0; j < beta / 2; j++) {
                int j_prime = floor(j / pow(2, i)) * pow(2, i);
                
                int first_bucket = j + j_prime;
                int second_bucket = j + j_prime + pow(2, i);
                
                binPlace(&temp, &labels_temp, A_in[first_bucket], A_in[second_bucket], 
                        labels_in[first_bucket], labels_in[second_bucket], i);
                        
                A_out[2 * j] = temp[0];
                A_out[2 * j + 1] = temp[1];
                labels_out[2 * j] = labels_temp[0];
                labels_out[2 * j + 1] = labels_temp[1];
            }

            A_in = A_out;
            labels_in = labels_out;
        }
        
        return A_out;
    }
 
}
