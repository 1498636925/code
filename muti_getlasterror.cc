#include <boost/circular_buffer.hpp>
#include <boost/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>

#include <iostream>

using namespace std;
using namespace boost;

#define MAX_CIRC_SIZE 16 
struct errorInfo {
	circular_buffer<string> err_mesg_clist;
	circular_buffer<int>    err_code_clist;
	//for c style output
	vector<const char*> cstrings;	
	vector<int> cints;
	
	errorInfo() {
		err_mesg_clist.set_capacity(MAX_CIRC_SIZE);
		err_code_clist.set_capacity(MAX_CIRC_SIZE);
		cstrings.clear();
		cints.clear();
	}
};


class CErrMsg {
	private:
		thread_specific_ptr<errorInfo> this_thread_buffer;
    public:
		void putMsg(const string& inMsg, const int inErrcode) {
			if (this_thread_buffer.get() == NULL) {
				this_thread_buffer.reset(new errorInfo);
			}
			errorInfo* pbuff = this_thread_buffer.get();
			pbuff->err_mesg_clist.push_back(inMsg);
			pbuff->err_code_clist.push_back(inErrcode);
		}

		void getMsg(const char** * outMsg, int*  *outErrcode, int* const outSize) {
			errorInfo* pbuff = this_thread_buffer.get();
			if (pbuff == NULL) {
				*outSize = 0;
				return ;	
			}
			pbuff->cstrings.clear();
			pbuff->cints.clear();

			for(circular_buffer<string>::iterator itr = pbuff->err_mesg_clist.begin();
					itr != pbuff->err_mesg_clist.end(); itr++) {
				pbuff->cstrings.push_back(itr->c_str());
			}

			for(circular_buffer<int>::iterator itr = pbuff->err_code_clist.begin();
					itr != pbuff->err_code_clist.end(); itr++) {
				pbuff->cints.push_back(*itr);
			}

			*outSize = pbuff->cstrings.size();
			*outMsg = pbuff->cstrings.data();
			*outErrcode = pbuff->cints.data();
		}

};

typedef boost::serialization::singleton<CErrMsg> s_err_msg;
#define st_err_msg s_err_msg::get_mutable_instance()

void thd()
{
	string threadId = lexical_cast<std::string>(this_thread::get_id());
	string msg = threadId + ":" + "in thread!";
	for (int i = 0; i < 32; i++)
		st_err_msg.putMsg(msg.c_str(), i);	

	const char** msg_array = NULL;
	int*   code_array = NULL;
	int size = 0;
	st_err_msg.getMsg(&msg_array, &code_array, &size);
	for (int i = 0; i < size; i++) {
		cout<<msg_array[i]<<"\t"<<code_array[i]<<endl;	
	}
}

//g++ muti_getlasterror.cc -o muti_getlasterror -lboost_system -lboost_thread -lpthread  -g -O0
int main()
{
	//main 
	st_err_msg.putMsg("hello,world!", 0);	
	st_err_msg.putMsg("hello,world!", 1);	
	st_err_msg.putMsg("hello,world!", 2);	
	st_err_msg.putMsg("hello,world!", 3);	
	st_err_msg.putMsg("hello,world!", 4);	
	st_err_msg.putMsg("hello,world!", 14);	

	const char** msg_array = NULL;
	int*   code_array = NULL;
	int size = 0;
	st_err_msg.getMsg(&msg_array, &code_array, &size);
	for (int i = 0; i < size; i++) {
		cout<<msg_array[i]<<"\t"<<code_array[i]<<endl;	
	}
	
	//two threads
#if 1	
	thread thrd1(&thd);
	thread thrd2(&thd);

	thrd1.join();
	thrd2.join();
#endif
	return 0;
}
