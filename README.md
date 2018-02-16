# kseq_reader
A threadsafe + buffered fastq/fasta reading wrapper, based on kseq.h from Heng Li


## Compiling
  ````
      git clone --recursive https://github.com/edawson/kseq_reader  
      make  
      ## To use in your code, compile with -lksr (and make  
      ## sure the library is in your LD_LIBRARY_PATH and the  
      ## header in LD_INCLUDE_PATH
  ````
  
  ## Usage
  
  ```
      #include "kseq_reader.hpp"
      
      using namespace std;
      using namespace KSR; // Need to access kseq_reader items through namespace 'KSR'
      int main(){
        
        // You can construct a file, then open it
        KSEQ_Reader kr;
        kr.open("reads.fq");
        
        // Or use the overloaded constructor and pass your filename
        KSEQ_Reader kt("reads.fq");
        
        // You can change the number of reads read in at a time but only
        // BEFORE reading from the file.
        kr.buffer_size(1000);
        
        // Get the next 1000 reads
        ksequence_t seq;
        int num = 0;
        int ret = kr.get_next_buf(seq, num);
        
        // seq now contains a pointer to num ksequence_t's,
        // which are structs with the following members:
        //char* name;
        //int name_len;
        // char* quality;
        // int qual_len;
        // char* comment;
        // int comment_len;
        // char* sequence;
        // uint32_t length;
        
        // You can iterate over an entire file (max(buffer size, number of reads left)) like so:
        int l = 0;
        
        while(l == 0){
          kt.get_next_buf(seq, num);
          for (int i = 0; i < num; i++){
            cout << seq.name << endl;
          }
        }
        // get_next_buf will return non-zero if the file can't be read or there are no more lines,
        // and the loop will terminate.
        
        
        
        // You can also just get one ksequence_t at a time:
        kr.next(seq);
      }
      
```
