#ifndef H_KSEQ_R
#define H_KSEQ_R

#include "kseq.hpp"
#include <zlib.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
KSEQ_INIT(gzFile, gzread);

using namespace std;

namespace KSR{

    struct ksequence_t{
        char* name;
        int name_len;

        char* quality;
        int qual_len;

        char* comment;
        int comment_len;

        char* sequence;
        uint32_t length;
    };

class KSEQ_Reader{
    

    public:
        KSEQ_Reader();
        ~KSEQ_Reader();
        KSEQ_Reader(char* filename);
        void open(char* filename);
        void open(const char* filename);
        void open(string& filename);
        void buffer_size(int& bufsize);
        int buffer_size();
        //int next(ksequence_t* ksq);
        int next(ksequence_t& ksq);
        int get_next_buff(ksequence_t*& ksq, int& num);
        int get_next_sequence_buffer(vector<string>& seqs);

    private:
        /** Buffer size **/
        int b_size = 1000;
        /** Number of elements in buffer **/
        int buff_len = 0;
        /** index of current element in buffer **/
        int curr_pos = 0;
        /** An array containing sequence objects **/
        ksequence_t* buff;
        /** Is it safe to change the buffer size? **/
        bool safe_to_set_buf = true;
        /** Has the buffer been initialized? **/
        bool init = false;
        /** Has the buffer been emptied? **/
        bool finished = false;

        char* filename;
        gzFile fp;  
        kseq_t* kseq;

        /**
         * Read from filename
         * until either:
         * - the buffer is filled
         * - the EOF is reached
         * **/
        int read();
}; 

}
#endif
