#include "kseq_reader.hpp"

using namespace std;
namespace KSR{
        KSEQ_Reader::KSEQ_Reader(){
            this->buff = new ksequence_t[b_size];
        }

        KSEQ_Reader::KSEQ_Reader(char* filename){
            this->buff = new ksequence_t[b_size];
            open(filename);
        }

        KSEQ_Reader::~KSEQ_Reader(){
            #pragma omp critical
            {
                delete [] buff;
                if (init){
                    kseq_destroy(kseq);
                    gzclose(fp);
                }
            }
            

        }

        int KSEQ_Reader::read(){

            if (finished){
                this->buff_len = 0;
                this->curr_pos = 0;
                return 1;
            }
            /** If our buffer hasn't been
             * initialized, set some variables for safety
             * and allocate memory
             **/
            if (!init){
                #pragma omp atomic write
                this->safe_to_set_buf = false;
                #pragma omp critical
                {
                for (int i = 0; i < b_size; i++){
                    this->buff[i] = *(new ksequence_t());
                    }
                }
                #pragma omp atomic write
                this->curr_pos = 0;
                #pragma omp atomic write
                this->buff_len = 0;
            }



            /** If our buffer is full,
             *  reset our variables. We
             *  might want to flush our
             *  buffer, too.
             **/
            if (buff_len == b_size){
                #pragma omp atomic write
                this->curr_pos = 0;
                #pragma omp atomic write
                this->buff_len = 0;
            }
            
            int ks_stat = 0;
            while (this->buff_len < b_size && 
            (ks_stat = kseq_read(kseq)) >= 0){
                char* name_x = new char[kseq->name.l];
                memcpy(name_x, kseq->name.s, kseq->name.l);
                this->buff[this->buff_len].name = name_x;
                this->buff[this->buff_len].name_len = kseq->name.l;

                char* x = new char[kseq->seq.l];
                memcpy(x, kseq->seq.s, kseq->seq.l);
                this->buff[this->buff_len].sequence = x;
                this->buff[this->buff_len].length = kseq->seq.l;
                
                this->buff_len = this->buff_len + 1;
            }

            if (ks_stat < 0){
               this->finished = true;
            }
            return 0;

        }

        void KSEQ_Reader::open(const char* filename){
            open((char*) filename);
        }

        void KSEQ_Reader::open(string& filename){
            open(filename.c_str());
        }

        void KSEQ_Reader::open(char* filename){
            this->filename = filename;
            this->fp = gzopen(this->filename, "r");
            if (this->fp == NULL){
                cerr << "Could not open " << filename << "." << endl;
                cerr << "Check the filename and that it is either plain text or gzip compressed." << endl;
                exit(2);
            }
            this->kseq = kseq_init(this->fp);
            this->init = true;
        }

        void KSEQ_Reader::buffer_size(int& bufsize){
            if (safe_to_set_buf){
                this->b_size = bufsize;
                delete [] this->buff;     
                this->buff = new ksequence_t[b_size];        
            }
            else{
                cerr << "Cannot set buffer size once items are in the buffer" << endl;
                exit(2);
            }
        }

        int KSEQ_Reader::buffer_size(){
            return this->b_size;
        }

        // int KSEQ_Reader::next(ksequence_t* ksq){
        //         if (!this->finished && curr_pos < buff_len){
        //             #pragma omp critical
        //             {
        //                 ksq = buff + curr_pos;
        //                 curr_pos++;
        //             }
                    
        //             return 0;
        //         }
        //         else if (!this->finished){
        //             #pragma omp critical
        //             {
        //                 this->read();
        //                 ksq = buff + curr_pos;                      
        //                 curr_pos++;
        //             }
        //             return 0;
        //         }
        //         else{
        //             return 2;
        //         }
            
            
        // }

        int KSEQ_Reader::next(ksequence_t& ksq){
            if (!this->finished && curr_pos < buff_len){
                #pragma omp critical
                {
                    ksq = *(buff + curr_pos);
                    curr_pos++;
                }
                //cerr << "Case 1" << endl;
                return 0;
            }
            else if (!this->finished){
                #pragma omp critical
                {
                    this->read();
                    ksq = *(buff + curr_pos);         
                    curr_pos++;
                }
                //cerr << "Case 2" << endl;
                
                return 0;
            }
            else{
                return 2;
            }
        
        }

        int KSEQ_Reader::get_next_buff(ksequence_t*& ksq, int& num){
                int r = read();
                #pragma omp critical
                {
                    num = buff_len - curr_pos;
                    ksq = this->buff + curr_pos;
                }
                return r;
            }
}