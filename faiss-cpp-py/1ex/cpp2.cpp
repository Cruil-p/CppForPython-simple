#include <python3.8/Python.h>
#include <iostream>

using namespace std;

int main(){
    Py_Initialize();
    if( !Py_IsInitialized()){
        cout << "python init fail" << endl;
        return 0;
    }
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./script')");

    PyObject* pModule = PyImport_ImportModule("sayHello");
    if( pModule == NULL ){
        cout <<"module not found" << endl;
        return 1;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "say");
    if( !pFunc || !PyCallable_Check(pFunc)){
        cout <<"not found function add_num" << endl;
        return 0;
    
    PyObject_CallObject(pFunc, NULL );

    Py_Finalize();
    return 0;
}
