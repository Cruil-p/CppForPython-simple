// Python_Invoker.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

//#define BOOST_PYTHON_STATIC_LIB

//#include <boost/python.hpp>
#include <python3.8/Python.h>
#include <string>
//#include <windows.h>
#include <iostream>
#include <vector>
#include <numpy/arrayobject.h>
#include <stdlib.h>

void temp_func()
{
    import_array();
}

void str_replace(std::string &s1, const std::string &s2, const std::string &s3)
{
    std::string::size_type pos = 0;
    std::string::size_type a = s2.size();
    std::string::size_type b = s3.size();
    while ((pos = s1.find(s2, pos)) != std::string::npos)
    {
        s1.replace(pos, a, s3);
        pos += b;
    }
}

std::string GetCurrentDir()
{
    char *path = getcwd(NULL, 0);
    const char *model_path = strcat(path, "/");
    std::string cur_dir(model_path);

    return cur_dir;
}

/*
   std::string GetCurrentDir()
   {
   char path[MAX_PATH];


   if (GetModuleFileNameA(NULL, path, MAX_PATH) > 0)
   {
   std::string cur_dir(path);
   str_replace(cur_dir, "\\", "/");
   auto last_pos = cur_dir.find_last_of("/");
   cur_dir = cur_dir.substr(0, last_pos + 1);
   return cur_dir;
   }
   else
   return "";
   }
   */
bool RunPythonStatement()
{

    if (!Py_IsInitialized())
    {
        return false;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        PyRun_SimpleString("sys.stdout.write('Hello, Hello, Hello!\\n')\n");
        PyRun_SimpleString("sys.stdout.write('3 + 4 = {}\\n'.format(3+4))\n");
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }

    return true;
}

//
bool InvokeNonParasFuncByAPI(std::string path, std::string module, std::string func)
{

    if (!Py_IsInitialized())
    {
        return false;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        std::string importDir = "sys.path.append('" + path + "')\n";
        PyRun_SimpleString(importDir.c_str());

        PyObject *moduleName = PyUnicode_FromString(module.c_str());
        PyObject *pModule = PyImport_Import(moduleName);
        if (!pModule)
        {
            std::cout << "Import Module Failed!" << std::endl;
            return false;
        }

        PyObject *pFunc = PyObject_GetAttrString(pModule, func.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            std::cout << "Get Function Failed!" << std::endl;
            return false;
        }

        PyObject *pResult = PyObject_CallObject(pFunc, nullptr);

        if (!pResult)
        {
            std::cout << "Get Result of Function Failed!" << std::endl;
            return false;
        }
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }

    return true;
}

bool InvokeFuncbyAPIOri(std::string path, std::string module, std::string func, std::vector<int> paras)
{

    if (!Py_IsInitialized())
    {
        return false;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        std::string importDir = "sys.path.append('" + path + "')\n";
        PyRun_SimpleString(importDir.c_str());

        PyObject *moduleName = PyUnicode_FromString(module.c_str());
        PyObject *pModule = PyImport_Import(moduleName);
        if (!pModule)
        {
            std::cout << "Import Module Failed!" << std::endl;
            return false;
        }

        PyObject *pFunc = PyObject_GetAttrString(pModule, func.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            std::cout << "Get Function Failed!" << std::endl;
            return false;
        }

        PyObject *pResult = nullptr;

        PyObject *args = PyTuple_New((int)paras.size());
        for (int i = 0; i < (int)paras.size(); i++)
        {
            PyTuple_SetItem(args, i, PyLong_FromLong(paras[i]));
        }

        pResult = PyObject_CallObject(pFunc, args);

        if (!pResult)
        {
            std::cout << "Get Result of Function Failed!" << std::endl;
            return false;
        }
        //std::cout << args[0] << " " << args[1] << std::endl;
        std::cout << "Result = " << PyLong_AsLong(pResult) << " by python api" << std::endl;
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }

    return true;
}

bool InvokeFuncbyAPI(std::string path, std::string module, std::string func, std::vector<int> paras)
{

    if (!Py_IsInitialized())
    {
        return false;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        std::string importDir = "sys.path.append('" + path + "')\n";
        PyRun_SimpleString(importDir.c_str());

        PyObject *moduleName = PyUnicode_FromString(module.c_str());
        std::cout << path << std::endl;
        std::cout << module << std::endl;
        std::cout << module.c_str() << std::endl;
        PyObject *pModule = PyImport_Import(moduleName);
        if (!pModule)
        {
            std::cout << "Import Module Failed!" << std::endl;
            return false;
        }

        PyObject *pFunc = PyObject_GetAttrString(pModule, func.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            std::cout << "Get Function Failed!" << std::endl;
            return false;
        }

        PyObject *pResult = nullptr;

        PyObject *args = PyTuple_New((int)paras.size());
        for (int i = 0; i < (int)paras.size(); i++)
        {
            PyTuple_SetItem(args, i, PyLong_FromLong(paras[i]));
        }

        pResult = PyObject_CallObject(pFunc, args);

        if (!pResult)
        {
            std::cout << "Get Result of Function Failed!" << std::endl;
            return false;
        }
        //

        //

        //import_array();
        temp_func();
        PyArrayObject *ret_array;
        PyArray_OutputConverter(pResult, &ret_array);
        npy_intp *shape = PyArray_SHAPE(ret_array);
        float *array = (float *)PyArray_DATA(ret_array);
        std::vector<float> ret_vector;
        ret_vector.insert(ret_vector.begin(), array, array + shape[0] * shape[1]);
        std::cout << "vector len:" << ret_vector.size() << std::endl;
        std::cout << "vector len:" << ret_vector.data()[7] << std::endl;

        std::cout << "shape:" << shape[0] << "," << shape[1] << std::endl;
        std::cout << "array:" << array[0] << "," << array[1] << std::endl;

        // std::cout << "Result = " << PyLong_AsLong(pResult) << " by python api" << std::endl;
    }

    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }

    return true;
}

PyObject *TrainAndGetCenter(std::string path, std::string module, std::string func, std::vector<int> paras, std::vector<double> data, std::vector<float> &clusCenters)
{

    if (!Py_IsInitialized())
    {
        return nullptr;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        std::string importDir = "sys.path.append('" + path + "')\n";
        PyRun_SimpleString(importDir.c_str());

        PyObject *moduleName = PyUnicode_FromString(module.c_str());
        PyObject *pModule = PyImport_Import(moduleName);
        if (!pModule)
        {
            std::cout << "Import Module Failed!" << std::endl;
            return nullptr;
        }

        PyObject *pFunc = PyObject_GetAttrString(pModule, func.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            std::cout << "Get Function Failed!" << std::endl;
            return nullptr;
        }

        //import_array();
        temp_func();

        double *pdata;

        npy_intp m = data.size();
        PyArrayObject *in_array = (PyArrayObject *)PyArray_SimpleNewFromData(1, &m, PyArray_DOUBLE, (double *)data.data());

        PyObject *args = PyTuple_New(3);
        PyTuple_SetItem(args, 0, PyLong_FromLong(paras[0]));
        PyTuple_SetItem(args, 1, PyLong_FromLong(paras[1]));
        PyTuple_SetItem(args, 2, (PyObject *)in_array);

        PyObject *pResult = PyObject_CallObject(pFunc, args);

        if (!pResult)
        {
            std::cout << "Get Result of Function Failed!" << std::endl;
            return nullptr;
        }
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return nullptr;
    }

    return nullptr;
}

bool getObjKmeans(std::string path, std::string module, std::string func, std::vector<int> paras, std::vector<float> data, PyObject* &pResult)
{

    if (!Py_IsInitialized())
    {
        return false;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        //PyRun_SimpleString("import sklearn\n");
        //PyRun_SimpleString("from sklearn.cluster import MiniBatchKMeans\n");
        std::string importDir = "sys.path.append('" + path + "')\n";
        PyRun_SimpleString(importDir.c_str());

        PyObject *moduleName = PyUnicode_FromString(module.c_str());
        //std::cout << path << std::endl;
        std::cout << PyUnicode_FromString(module.c_str())<<std::endl;
        std::cout << module.c_str() << std::endl;
        PyObject *pModule = PyImport_Import(moduleName);
        std::cout << module << "sss" << module.c_str() << std::endl;
        //std::cout << PyImport_Import(moduleName) << std::endl;
        if (!pModule)
        {
            //std::cout << pModule << std::endl;
            std::cout << "Import Module Failed!" << std::endl;
            return false;
        }

        PyObject *pFunc = PyObject_GetAttrString(pModule, func.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            std::cout << "Get Function Failed!" << std::endl;
            return false;
        }

        //import_array();
        temp_func();

        //double *pdata;

        npy_intp m = data.size();
        PyArrayObject * in_array = (PyArrayObject*)PyArray_SimpleNewFromData(1, &m, PyArray_FLOAT, (float*)data.data());

        PyObject* args = PyTuple_New(3);
        PyTuple_SetItem(args, 0, PyLong_FromLong(paras[0]));
        PyTuple_SetItem(args, 1, PyLong_FromLong(paras[1]));
        PyTuple_SetItem(args, 2,(PyObject*) in_array);

        pResult = PyObject_CallObject(pFunc, args);

        if (!pResult)
        {
            std::cout << "Get Result of Function Failed!" << std::endl;
            return false;
        }
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }

    return true;
}

PyObject *GetTrained(std::string path, std::string module, std::string func, std::vector<int> paras, std::vector<float> data)
{
    PyObject *pResult;
    if (!Py_IsInitialized())
    {
        return nullptr;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        PyRun_SimpleString("from sklearn.cluster import MiniBatchKMeans\n");
        std::string importDir = "sys.path.append('" + path + "')\n";
        PyRun_SimpleString(importDir.c_str());

        PyObject *moduleName = PyUnicode_FromString(module.c_str());
        PyObject *pModule = PyImport_Import(moduleName);
        if (!pModule)
        {
            std::cout << "Import Module Failed!" << std::endl;
            return nullptr;
        }

        PyObject *pFunc = PyObject_GetAttrString(pModule, func.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            std::cout << "Get Function Failed!" << std::endl;
            return nullptr;
        }

        //import_array();
        temp_func();

        double *pdata;

        npy_intp m = data.size();

        PyArrayObject *in_array = (PyArrayObject *)PyArray_SimpleNewFromData(1, &m, PyArray_DOUBLE, (double *)data.data());

        PyObject *args = PyTuple_New(3);
        PyTuple_SetItem(args, 0, PyLong_FromLong(paras[0]));
        PyTuple_SetItem(args, 1, PyLong_FromLong(paras[1]));
        PyTuple_SetItem(args, 2, (PyObject *)in_array);

        pResult = PyObject_CallObject(pFunc, args);

        if (!pResult)
        {
            std::cout << "Get Result of Function Failed!" << std::endl;
            return nullptr;
        }

        //
        //

        //
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return nullptr;
    }

    return pResult;
}

bool getClusCenters(std::string path, std::string module, std::string func, PyObject *objKmeans, std::vector<float> &clusCenters)
{

    if (!Py_IsInitialized())
    {
        return false;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        //PyRun_SimpleString("from sklearn.cluster import MiniBatchKMeans\n");
        std::string importDir = "sys.path.append('" + path + "')\n";
        PyRun_SimpleString(importDir.c_str());

        PyObject *moduleName = PyUnicode_FromString(module.c_str());
        PyObject *pModule = PyImport_Import(moduleName);
        if (!pModule)
        {
            std::cout << "Import Module Failed!" << std::endl;
            return false;
        }

        PyObject *pFunc = PyObject_GetAttrString(pModule, func.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            std::cout << "Get Function Failed!" << std::endl;
            return false;
        }

        //import_array();
        temp_func();

        PyObject *args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, (PyObject *)objKmeans);

        PyObject *pResult = PyObject_CallObject(pFunc, args);

        if (!pResult)
        {
            std::cout << "Get Result of Function Failed!" << std::endl;
            return false;
        }

        PyArrayObject *ret_array;
        PyArray_OutputConverter(pResult, &ret_array);
        npy_intp *shape = PyArray_SHAPE(ret_array);
        float *array = (float *)PyArray_DATA(ret_array);

        clusCenters.insert(clusCenters.begin(), array, array + shape[0] * shape[1]);
        std::cout << "vector len:" << clusCenters.size() << std::endl;
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }

    return true;
}

bool addData(std::string path, std::string module, std::string func, PyObject *objKmeans, std::vector<float> &data, std::vector<int> paras, std::vector<int> &idxtotal)
{

    if (!Py_IsInitialized())
    {
        return false;
    }

    try
    {
        PyRun_SimpleString("import sys\n");
        std::string importDir = "sys.path.append('" + path + "')\n";
        PyRun_SimpleString(importDir.c_str());

        PyObject *moduleName = PyUnicode_FromString(module.c_str());
        PyObject *pModule = PyImport_Import(moduleName);
        if (!pModule)
        {
            std::cout << "Import Module Failed!" << std::endl;
            return false;
        }

        PyObject *pFunc = PyObject_GetAttrString(pModule, func.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            std::cout << "Get Function Failed!" << std::endl;
            return false;
        }

        //import_array();
        temp_func();
        float *pdata;

        npy_intp m = data.size();
        PyArrayObject *in_data = (PyArrayObject *)PyArray_SimpleNewFromData(1, &m, PyArray_FLOAT, (float *)data.data());
        m = idxtotal.size();
        std::cout << "idxtotal.size: " << m << std::endl;
        std::cout << "dim: " << paras[1] << std::endl;
        PyArrayObject *in_idxtotal = (PyArrayObject *)PyArray_SimpleNewFromData(1, &m, PyArray_INT, (int *)idxtotal.data());

        PyObject *args = PyTuple_New(3);
        PyTuple_SetItem(args, 0, (PyObject *)objKmeans);
        PyTuple_SetItem(args, 1, (PyObject *)in_data);
        PyTuple_SetItem(args, 2, PyLong_FromLong(paras[1]));
        //

        PyObject *pResult = PyObject_CallObject(pFunc, args);

        if (!pResult)
        {
            std::cout << "Get Result of Function Failed!" << std::endl;
            return false;
        }

        PyArrayObject *ret_array;
        PyArray_OutputConverter(pResult, &ret_array);
        npy_intp *shape = PyArray_SHAPE(ret_array);
        std::cout << "shape[0]: " << shape[0] << std::endl;
        int *array = (int *)PyArray_DATA(ret_array);

        idxtotal.insert(idxtotal.begin(), array, array + shape[0]);
        std::cout << "idxtotal len:" << idxtotal.size() << std::endl;
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }

    return true;
}

int main()
{
    std::cout << "----Run Python Statement----" << std::endl;
    // RunPythonStatement();
    // GetCurrentDir();

    int dim = 64;//64
    int ntotal = 8192;//8192
    int ncentroids = 8192;//8192
    int nprobe = 64;//8192

    std::vector<float> data(dim * ntotal);
    for (int i = 0; i < dim * ntotal; i++)
    {
        data[i] = drand48();
    }

    Py_Initialize();
    //PyRun_SimpleString("from sklearn.cluster import MiniBatchKMeans");
    //PyRun_SimpleString("import sklearn");

    PyObject *objKmeans;
    //释放内存PyObject
    std::vector<int> paras;
    paras.push_back(ncentroids);
    paras.push_back(dim);

    std::vector<float> clusCenters;
    std::vector<int> idxtotal;
    std::cout << "----Invoke Python Script by API----" << std::endl;
    /*std::cout << "----Invoke Python Script by API----" << std::endl;*/
      InvokeNonParasFuncByAPI(GetCurrentDir() + "pyscripts", "PythonGreet", "Hello");
    //InvokeFuncbyAPI(GetCurrentDir() + "pyscripts", "PythonCalc", "Add", paras);/**/
    // std::cout << "----Invoke Python Script by boost::python----" << std::endl;

    InvokeFuncbyAPIOri(GetCurrentDir() + "pyscripts", "PythonCalc", "Add", paras);
    InvokeFuncbyAPIOri(GetCurrentDir() + "pyscripts", "PythonCalc", "Add", paras);
    InvokeFuncbyAPI(GetCurrentDir() + "pyscripts", "PythonCalc", "retArray", paras);
    getObjKmeans(GetCurrentDir() + "pyscripts", "mmmmm", "getObjKmeans", paras, data, objKmeans);
    //
    //
    //
    //
    //
    getClusCenters(GetCurrentDir() + "pyscripts", "mmmmm", "getClusCenters", objKmeans, clusCenters);
    addData(GetCurrentDir() + "pyscripts", "mmmmm", "getClusIndex", objKmeans, data, paras, idxtotal);
    Py_Finalize();
    // getObjKmeans(GetCurrentDir()+ "pyscripts", "", "", paras, data, objKmeans);
    // InvokeFuncbyBoost(GetCurrentDir() + "pyscripts", "PythonCalc", "Add", paras);

    return 0;
}

// bool InvokeNonParasFuncByBoost(std::string path, std::string module, std::string func)
// {
//     Py_Initialize();

//     if (!Py_IsInitialized())
//     {
//         return false;
//     }

//     try
//     {
//         PyRun_SimpleString("import sys\n");
//         std::string importDir = "sys.path.append('" + path + "')\n";
//         PyRun_SimpleString(importDir.c_str());

//         boost::python::object pyModule(boost::python::import(module.c_str()));
//         pyModule.attr(func.c_str())();

//     }
//     catch (...)
//     {
//         PyErr_Print();
//         PyErr_Clear();
//         Py_Finalize();
//         return false;
//     }
//     Py_Finalize();
//     return true;
// }

// bool InvokeFuncbyBoost(std::string path, std::string module, std::string func, std::vector<int> paras)
// {
//     Py_Initialize();

//     if (!Py_IsInitialized())
//     {
//         return false;
//     }

//     try
//     {
//         PyRun_SimpleString("import sys\n");
//         std::string importDir = "sys.path.append('" + path + "')\n";
//         PyRun_SimpleString(importDir.c_str());

//         boost::python::object pyModule(boost::python::import(module.c_str()));
//         boost::python::object pyResult(pyModule.attr(func.c_str())(boost::python::object(paras[0]), boost::python::object(paras[1])));
//         std::cout << "Result = " << boost::python::extract<int32_t>(pyResult) << " by boost::python" << std::endl;

//     }
//     catch (...)
//     {
//         PyErr_Print();
//         PyErr_Clear();
//         Py_Finalize();
//         return false;
//     }
//     Py_Finalize();
//     return true;
// }
