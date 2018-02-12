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
    string k ("small.fq.gz");
    KSEQ_Reader g;
    g.open(k);
    int bs = 110;
    g.buffer_size(bs);
    cout << "Test 2: constructor 2 pass. Bsize: " << g.buffer_size() << endl;
    ksequence_t seq;
    //int r = g.next(seq);
    //cout << "Test 3: next(): " << r << " " << endl;
    //cout << "Test sequence: " << seq.name << "\n" <<
    //seq.sequence << " successful, seqlen " << seq.length << endl;
    ksequence_t* kpt;
    int kpt_num;
    //int l = g.get_next_buff(kpt, kpt_num);
    //cerr << l << " kpt " << kpt_num << endl;
    int l = g.get_next_buff(kpt, kpt_num);
    while (l == 0){
        for (int i = 0; i < kpt_num; i++){
            cout << (kpt + i)->name << endl;
        }
        l = g.get_next_buff(kpt, kpt_num);
    }
    

return 0;
}
