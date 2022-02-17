#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *crypto_sign_keypair_python(PyObject *self, PyObject *args)
{
    PyBytesObject *seed;
    uint8_t *pubkey;
    uint8_t *privkey;
    PyObject *private_key;
    PyObject *public_key;
    PyObject *rslt = PyTuple_New(2);

    if (!PyArg_ParseTuple(args, "S", &seed))
        return NULL;
    Py_INCREF(seed);

    pubkey = PyMem_Malloc(1952);
    privkey = PyMem_Malloc(4000);

    PQCLEAN_DILITHIUM3_CLEAN_crypto_sign_keypair_seed(pubkey, privkey, (uint8_t *)PyBytes_AsString((PyObject*) seed));

    Py_DECREF(seed);

    private_key = Py_BuildValue("y#", privkey, 4000);
    public_key = Py_BuildValue("y#", pubkey, 1952);

    PyMem_Free(pubkey);
    PyMem_Free(privkey);

    PyTuple_SetItem(rslt,0,public_key);
    PyTuple_SetItem(rslt,1,private_key);

    return rslt;
}

static PyObject *crypto_sign_keypair_random_python(PyObject *self)
{
    PyBytesObject *seed;
    uint8_t *pubkey;
    uint8_t *privkey;
    PyObject *private_key;
    PyObject *public_key;
    PyObject *rslt = PyTuple_New(2);

    pubkey = PyMem_Malloc(1952);
    privkey = PyMem_Malloc(4000);

    PQCLEAN_DILITHIUM3_CLEAN_crypto_sign_keypair(pubkey, privkey);

    private_key = Py_BuildValue("y#", privkey, 4000);
    public_key = Py_BuildValue("y#", pubkey, 1952);

    PyMem_Free(pubkey);
    PyMem_Free(privkey);

    PyTuple_SetItem(rslt,0,public_key);
    PyTuple_SetItem(rslt,1,private_key);

    return rslt;
}


static PyObject *crypto_sign_signature_python(PyObject *self, PyObject *args)
{
    PyBytesObject *m;
    PyBytesObject *sk;
    uint8_t *sig;
    size_t siglen;
    PyObject *output_sig;

    if (!PyArg_ParseTuple(args, "SS", &sk, &m))
        return NULL;    
    Py_INCREF(sk);        
    Py_INCREF(m);
    

    sig = PyMem_Malloc(3293);

    PQCLEAN_DILITHIUM3_CLEAN_crypto_sign_signature(sig, &siglen, (uint8_t *)PyBytes_AsString((PyObject*) m), PyBytes_GET_SIZE(m), (uint8_t *)PyBytes_AsString((PyObject*) sk));
    
    Py_DECREF(m);    
    Py_DECREF(sk);

    
    output_sig = Py_BuildValue("y#", sig, siglen);

    PyMem_Free(sig);

    return output_sig;
}

static PyObject *crypto_sign_verify_python(PyObject *self, PyObject *args)
{
    PyBytesObject *m;
    PyBytesObject *sig;
    PyBytesObject *pubkey;
    int result;
    

    if (!PyArg_ParseTuple(args, "SSS", &pubkey, &m, &sig))
        return NULL;
    Py_INCREF(pubkey);
    Py_INCREF(m);
    Py_INCREF(sig);
    
    result = PQCLEAN_DILITHIUM3_CLEAN_crypto_sign_verify((uint8_t *)PyBytes_AsString((PyObject*) sig), PyBytes_GET_SIZE(sig), (uint8_t *)PyBytes_AsString((PyObject*) m), PyBytes_GET_SIZE(m), (uint8_t *)PyBytes_AsString((PyObject*) pubkey));
    
    Py_DECREF(sig);
    Py_DECREF(m);
    Py_DECREF(pubkey);
    

    if(result<0) Py_RETURN_FALSE;
    else Py_RETURN_TRUE;

}

static PyMethodDef arl_dilithiumMethods[] = {
    { "generate_keypair", (PyCFunction)crypto_sign_keypair_python, METH_VARARGS, "crypto_sign_keypair_python" },
    { "generate_keypair_random", (PyCFunction)crypto_sign_keypair_random_python, METH_VARARGS, "crypto_sign_keypair_random_python" },
    { "sign", (PyCFunction)crypto_sign_signature_python, METH_VARARGS, "crypto_sign_signature_python" },
    { "verify", (PyCFunction)crypto_sign_verify_python, METH_VARARGS, "crypto_sign_verify_python" },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef arl_dilithiumModule = {
    PyModuleDef_HEAD_INIT,
    "arl_dilithium",
    "Dilithium3 bindings for ArielCoin",
    -1,
    arl_dilithiumMethods
};

PyMODINIT_FUNC PyInit_arl_dilithium(void) {
    return PyModule_Create(&arl_dilithiumModule);
}