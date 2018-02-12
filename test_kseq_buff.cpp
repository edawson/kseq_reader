#include "KSEQ_Reader.hpp"
#include <iostream>

using namespace std;
using namespace KSR;
int main(){
    // Hacky test for uninitialized constructor/destructor
    if (true){
        KSEQ_Reader ksr;
        cout << "Test 1: constructor pass" << endl;
    }
    string k ("SRR4074258.fastq.gz");
    KSEQ_Reader g;
    g.open(k);
    int bs = 200;
    g.buffer_size(bs);
    cout << "Test 2: constructor 2 pass. Bsize: " << g.buffer_size() << endl;
    ksequence_t seq;
    int r = g.next(seq);
    cout << "Test 3: next(): " << r << " " << endl;
    cout << "Test sequence: " << seq.name << "\n" <<
    seq.sequence << " successful, seqlen " << seq.length << endl;
    // ksequence_t* kpt;
    // int kpt_num;
    // int l = g.get_next_buff(kpt, kpt_num);
    // while (l){
    //     l = g.get_next_buff(kpt, kpt_num);
    //     for (int i = 0; i < kpt_num; i++){
    //         cout << (*(kpt + i)).sequence << endl;
    //     }
    // }
    

return 0;
}
