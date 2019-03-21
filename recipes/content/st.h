/* -*- mode: c; coding: utf-8 -*- */

#include <IceC/IceC.h>
#include <IceC/IceUtil.h>

typedef struct st_IBool {
    Ice_Object __base;
} st_IBool;

typedef st_IBool* st_IBoolPtr;

typedef Ice_ObjectPrx st_IBoolPrx;
typedef Ice_ObjectPrx* st_IBoolPrxPtr;

/* forward declaration */
static void
st_IBool_methodHandler(st_IBoolPtr self,
                       Ice_InputStreamPtr is,
                       Ice_OutputStreamPtr os);

static void
st_IBool_init(st_IBoolPtr self) {

    trace();
    
    Ice_Object_init((Ice_ObjectPtr)self,
            (Ice_MethodHandlerPtr)st_IBool_methodHandler);
}

void
st_IBoolI_set(st_IBoolPtr self,
              Ice_Bool v,
              Ice_String sourceAddr) __attribute__((weak));

static void
st_IBool_set__dispatch(st_IBoolPtr self,
                       Ice_InputStreamPtr is,
                       Ice_OutputStreamPtr os) {
    Ice_Bool v = 0;
    Ice_String sourceAddr;

    trace();
    
    if (&st_IBoolI_set == 0) {
        return;
    }
    Ice_InputStream_readBool(is, &v);
    
    Ice_InputStream_readIceString(is, &sourceAddr);
    
    st_IBoolI_set(self, v, sourceAddr);
}

static void
st_IBool_methodHandler(st_IBoolPtr self,
                       Ice_InputStreamPtr is,
                       Ice_OutputStreamPtr os) {
    Ice_Int operationSize = 0;
    char operation[MAX_OPERATION_NAME_SIZE];
    byte mode = 0;
    byte contextSize = 0;
    Ice_Int encapSize = 0;
    byte major = 0;
    byte minor = 0;

    trace();
    
    Ptr_check(self);
    Ptr_check(is);
    Ptr_check(os);
    
    Ice_InputStream_readSize(is, &operationSize, false);
    assert(operationSize <= MAX_OPERATION_NAME_SIZE);
    
    Ice_InputStream_readString(is, operation);
    
    /* FIXME: operationMode ignored */
    Ice_InputStream_readByte(is, &mode);
    
    /* FIXME: Context not supported */
    Ice_InputStream_readByte(is, &contextSize);
    assert(contextSize == 0);
    
    Ice_InputStream_readInt(is, &encapSize);
    Ice_InputStream_readByte(is, &major);
    Ice_InputStream_readByte(is, &minor);
    assert(major == 1 && minor == 0);
    assert(encapSize - 6 == (is->size - (is->next - is->data)));
    
    if (strcmp(operation, "set") == 0) {
        st_IBool_set__dispatch(self, is, os);
        return;
    }
    
    c_throw("Ice_OperationNotExistException", __FILE__, __LINE__);
}

static void
st_IBool_set(st_IBoolPrxPtr self,
             Ice_Bool v,
             Ice_String sourceAddr) {
    Ice_OutputStreamPtr os = &(self->stream);

    trace();
    
    Ptr_check(self);
    
    Ice_ObjectPrx_connect(self);
    Ice_OutputStream_init(os);
    
    /* request header */
    Ice_OutputStream_writeBlob(os, Ice_header, sizeof(Ice_header));
    
    /* request body */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 1);
    }
    if (self->mode != Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 0);
    }
    
    Ice_OutputStream_writeIdentity(os, self->identity);
    Ice_OutputStream_writeString(os, "");
    Ice_OutputStream_writeString(os, "set");
    Ice_OutputStream_writeByte(os, Ice_Normal);
    Ice_OutputStream_writeByte(os, 0);
    
    /* encapsulated params */
    Ice_OutputStream_startWriteEncaps(os);
    Ice_OutputStream_writeBool(os, v);
    Ice_OutputStream_writeIceString(os, sourceAddr);
    Ice_OutputStream_endWriteEncaps(os);
    Ice_OutputStream_setMessageSize(os);
    
    Ice_ObjectPrx_send(self);
    
    /* parse reply message (only on twoway invocations) */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_InputStream is;
        char expected[10] = {'I', 'c', 'e', 'P', 1, 0, 1, 0, Ice_replyMsg, 0};
        Ice_Int size = 0;
        Ice_Int requestId = 0;
        Ice_Byte replyStatus = 0;
        Ice_Int encapsSize = 0;
        Ice_Byte skipByte = 0;
        
        uint16_t timeout = 5000;
        bool ready = false;
        while (!ready) {
            IcePlugin_Connection_dataReady(&(self->connection), &ready);
            IceUtil_delay(1);
            if (! --timeout) {
                c_throw("Ice_TimeoutException", __FILE__, __LINE__);
                return;
            }
        }
        
        /* reply header */
        Ice_InputStream_init(&is, self->connection.fd, self->connection.epinfo->type);
        if (memcmp(expected, is.next, 10) != 0) {
            c_throw("Ice_ProtocolException", __FILE__, __LINE__);
            return;
        }
        
        /* skip header from input stream */
        is.next += 10;
        Ice_InputStream_readInt(&is, &size);
            if (size > MAX_MESSAGE_SIZE) {
            c_throw("Ice_MemoryLimitException", __FILE__, __LINE__);
            return;
        }
        
        /* reply body */
        Ice_InputStream_readInt(&is, &requestId);
            Ice_InputStream_readByte(&is, &replyStatus);
            
        /* check replyStatus */
        switch (replyStatus) {
        case 0: {
            /* success */
            break;
        }
        case 1: {
            /* user exception */
            Ice_ObjectPrx_parseUserException(self, &is);
            return;
        }
        case 2: {
            c_throw("Ice_ObjectDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 3: {
            c_throw("Ice_FacetDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 4: {
            c_throw("Ice_OperationDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 5:
        case 6:
        case 7: {
            /* unkown exception (reason as string) */
            /* FIXME: unmarshall and print reason */
            c_throw("Ice_UnknownException", __FILE__, __LINE__);
            return;
        }}
        
        Ice_InputStream_readInt(&is, &encapsSize);
            Ice_InputStream_readByte(&is, &skipByte);
            Ice_InputStream_readByte(&is, &skipByte);
        }
    
    Ice_ObjectPrx_closeConnection(self);
}

typedef struct st_IByte {
    Ice_Object __base;
} st_IByte;

typedef st_IByte* st_IBytePtr;

typedef Ice_ObjectPrx st_IBytePrx;
typedef Ice_ObjectPrx* st_IBytePrxPtr;

/* forward declaration */
static void
st_IByte_methodHandler(st_IBytePtr self,
                       Ice_InputStreamPtr is,
                       Ice_OutputStreamPtr os);

static void
st_IByte_init(st_IBytePtr self) {

    trace();
    
    Ice_Object_init((Ice_ObjectPtr)self,
            (Ice_MethodHandlerPtr)st_IByte_methodHandler);
}

void
st_IByteI_set(st_IBytePtr self,
              Ice_Byte v,
              Ice_String sourceAddr) __attribute__((weak));

static void
st_IByte_set__dispatch(st_IBytePtr self,
                       Ice_InputStreamPtr is,
                       Ice_OutputStreamPtr os) {
    Ice_Byte v = 0;
    Ice_String sourceAddr;

    trace();
    
    if (&st_IByteI_set == 0) {
        return;
    }
    Ice_InputStream_readByte(is, &v);
    
    Ice_InputStream_readIceString(is, &sourceAddr);
    
    st_IByteI_set(self, v, sourceAddr);
}

static void
st_IByte_methodHandler(st_IBytePtr self,
                       Ice_InputStreamPtr is,
                       Ice_OutputStreamPtr os) {
    Ice_Int operationSize = 0;
    char operation[MAX_OPERATION_NAME_SIZE];
    byte mode = 0;
    byte contextSize = 0;
    Ice_Int encapSize = 0;
    byte major = 0;
    byte minor = 0;

    trace();
    
    Ptr_check(self);
    Ptr_check(is);
    Ptr_check(os);
    
    Ice_InputStream_readSize(is, &operationSize, false);
    assert(operationSize <= MAX_OPERATION_NAME_SIZE);
    
    Ice_InputStream_readString(is, operation);
    
    /* FIXME: operationMode ignored */
    Ice_InputStream_readByte(is, &mode);
    
    /* FIXME: Context not supported */
    Ice_InputStream_readByte(is, &contextSize);
    assert(contextSize == 0);
    
    Ice_InputStream_readInt(is, &encapSize);
    Ice_InputStream_readByte(is, &major);
    Ice_InputStream_readByte(is, &minor);
    assert(major == 1 && minor == 0);
    assert(encapSize - 6 == (is->size - (is->next - is->data)));
    
    if (strcmp(operation, "set") == 0) {
        st_IByte_set__dispatch(self, is, os);
        return;
    }
    
    c_throw("Ice_OperationNotExistException", __FILE__, __LINE__);
}

static void
st_IByte_set(st_IBytePrxPtr self,
             Ice_Byte v,
             Ice_String sourceAddr) {
    Ice_OutputStreamPtr os = &(self->stream);

    trace();
    
    Ptr_check(self);
    
    Ice_ObjectPrx_connect(self);
    Ice_OutputStream_init(os);
    
    /* request header */
    Ice_OutputStream_writeBlob(os, Ice_header, sizeof(Ice_header));
    
    /* request body */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 1);
    }
    if (self->mode != Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 0);
    }
    
    Ice_OutputStream_writeIdentity(os, self->identity);
    Ice_OutputStream_writeString(os, "");
    Ice_OutputStream_writeString(os, "set");
    Ice_OutputStream_writeByte(os, Ice_Normal);
    Ice_OutputStream_writeByte(os, 0);
    
    /* encapsulated params */
    Ice_OutputStream_startWriteEncaps(os);
    Ice_OutputStream_writeByte(os, v);
    Ice_OutputStream_writeIceString(os, sourceAddr);
    Ice_OutputStream_endWriteEncaps(os);
    Ice_OutputStream_setMessageSize(os);
    
    Ice_ObjectPrx_send(self);
    
    /* parse reply message (only on twoway invocations) */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_InputStream is;
        char expected[10] = {'I', 'c', 'e', 'P', 1, 0, 1, 0, Ice_replyMsg, 0};
        Ice_Int size = 0;
        Ice_Int requestId = 0;
        Ice_Byte replyStatus = 0;
        Ice_Int encapsSize = 0;
        Ice_Byte skipByte = 0;
        
        uint16_t timeout = 5000;
        bool ready = false;
        while (!ready) {
            IcePlugin_Connection_dataReady(&(self->connection), &ready);
            IceUtil_delay(1);
            if (! --timeout) {
                c_throw("Ice_TimeoutException", __FILE__, __LINE__);
                return;
            }
        }
        
        /* reply header */
        Ice_InputStream_init(&is, self->connection.fd, self->connection.epinfo->type);
        if (memcmp(expected, is.next, 10) != 0) {
            c_throw("Ice_ProtocolException", __FILE__, __LINE__);
            return;
        }
        
        /* skip header from input stream */
        is.next += 10;
        Ice_InputStream_readInt(&is, &size);
            if (size > MAX_MESSAGE_SIZE) {
            c_throw("Ice_MemoryLimitException", __FILE__, __LINE__);
            return;
        }
        
        /* reply body */
        Ice_InputStream_readInt(&is, &requestId);
            Ice_InputStream_readByte(&is, &replyStatus);
            
        /* check replyStatus */
        switch (replyStatus) {
        case 0: {
            /* success */
            break;
        }
        case 1: {
            /* user exception */
            Ice_ObjectPrx_parseUserException(self, &is);
            return;
        }
        case 2: {
            c_throw("Ice_ObjectDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 3: {
            c_throw("Ice_FacetDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 4: {
            c_throw("Ice_OperationDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 5:
        case 6:
        case 7: {
            /* unkown exception (reason as string) */
            /* FIXME: unmarshall and print reason */
            c_throw("Ice_UnknownException", __FILE__, __LINE__);
            return;
        }}
        
        Ice_InputStream_readInt(&is, &encapsSize);
            Ice_InputStream_readByte(&is, &skipByte);
            Ice_InputStream_readByte(&is, &skipByte);
        }
    
    Ice_ObjectPrx_closeConnection(self);
}

typedef struct st_IFloat {
    Ice_Object __base;
} st_IFloat;

typedef st_IFloat* st_IFloatPtr;

typedef Ice_ObjectPrx st_IFloatPrx;
typedef Ice_ObjectPrx* st_IFloatPrxPtr;

/* forward declaration */
static void
st_IFloat_methodHandler(st_IFloatPtr self,
                        Ice_InputStreamPtr is,
                        Ice_OutputStreamPtr os);

static void
st_IFloat_init(st_IFloatPtr self) {

    trace();
    
    Ice_Object_init((Ice_ObjectPtr)self,
            (Ice_MethodHandlerPtr)st_IFloat_methodHandler);
}

void
st_IFloatI_set(st_IFloatPtr self,
               Ice_Float v,
               Ice_String sourceAddr) __attribute__((weak));

static void
st_IFloat_set__dispatch(st_IFloatPtr self,
                        Ice_InputStreamPtr is,
                        Ice_OutputStreamPtr os) {
    Ice_Float v = 0;
    Ice_String sourceAddr;

    trace();
    
    if (&st_IFloatI_set == 0) {
        return;
    }
    Ice_InputStream_readFloat(is, &v);
    
    Ice_InputStream_readIceString(is, &sourceAddr);
    
    st_IFloatI_set(self, v, sourceAddr);
}

static void
st_IFloat_methodHandler(st_IFloatPtr self,
                        Ice_InputStreamPtr is,
                        Ice_OutputStreamPtr os) {
    Ice_Int operationSize = 0;
    char operation[MAX_OPERATION_NAME_SIZE];
    byte mode = 0;
    byte contextSize = 0;
    Ice_Int encapSize = 0;
    byte major = 0;
    byte minor = 0;

    trace();
    
    Ptr_check(self);
    Ptr_check(is);
    Ptr_check(os);
    
    Ice_InputStream_readSize(is, &operationSize, false);
    assert(operationSize <= MAX_OPERATION_NAME_SIZE);
    
    Ice_InputStream_readString(is, operation);
    
    /* FIXME: operationMode ignored */
    Ice_InputStream_readByte(is, &mode);
    
    /* FIXME: Context not supported */
    Ice_InputStream_readByte(is, &contextSize);
    assert(contextSize == 0);
    
    Ice_InputStream_readInt(is, &encapSize);
    Ice_InputStream_readByte(is, &major);
    Ice_InputStream_readByte(is, &minor);
    assert(major == 1 && minor == 0);
    assert(encapSize - 6 == (is->size - (is->next - is->data)));
    
    if (strcmp(operation, "set") == 0) {
        st_IFloat_set__dispatch(self, is, os);
        return;
    }
    
    c_throw("Ice_OperationNotExistException", __FILE__, __LINE__);
}

static void
st_IFloat_set(st_IFloatPrxPtr self,
              Ice_Float v,
              Ice_String sourceAddr) {
    Ice_OutputStreamPtr os = &(self->stream);

    trace();
    
    Ptr_check(self);
    
    Ice_ObjectPrx_connect(self);
    Ice_OutputStream_init(os);
    
    /* request header */
    Ice_OutputStream_writeBlob(os, Ice_header, sizeof(Ice_header));
    
    /* request body */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 1);
    }
    if (self->mode != Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 0);
    }
    
    Ice_OutputStream_writeIdentity(os, self->identity);
    Ice_OutputStream_writeString(os, "");
    Ice_OutputStream_writeString(os, "set");
    Ice_OutputStream_writeByte(os, Ice_Normal);
    Ice_OutputStream_writeByte(os, 0);
    
    /* encapsulated params */
    Ice_OutputStream_startWriteEncaps(os);
    Ice_OutputStream_writeFloat(os, v);
    Ice_OutputStream_writeIceString(os, sourceAddr);
    Ice_OutputStream_endWriteEncaps(os);
    Ice_OutputStream_setMessageSize(os);
    
    Ice_ObjectPrx_send(self);
    
    /* parse reply message (only on twoway invocations) */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_InputStream is;
        char expected[10] = {'I', 'c', 'e', 'P', 1, 0, 1, 0, Ice_replyMsg, 0};
        Ice_Int size = 0;
        Ice_Int requestId = 0;
        Ice_Byte replyStatus = 0;
        Ice_Int encapsSize = 0;
        Ice_Byte skipByte = 0;
        
        uint16_t timeout = 5000;
        bool ready = false;
        while (!ready) {
            IcePlugin_Connection_dataReady(&(self->connection), &ready);
            IceUtil_delay(1);
            if (! --timeout) {
                c_throw("Ice_TimeoutException", __FILE__, __LINE__);
                return;
            }
        }
        
        /* reply header */
        Ice_InputStream_init(&is, self->connection.fd, self->connection.epinfo->type);
        if (memcmp(expected, is.next, 10) != 0) {
            c_throw("Ice_ProtocolException", __FILE__, __LINE__);
            return;
        }
        
        /* skip header from input stream */
        is.next += 10;
        Ice_InputStream_readInt(&is, &size);
            if (size > MAX_MESSAGE_SIZE) {
            c_throw("Ice_MemoryLimitException", __FILE__, __LINE__);
            return;
        }
        
        /* reply body */
        Ice_InputStream_readInt(&is, &requestId);
            Ice_InputStream_readByte(&is, &replyStatus);
            
        /* check replyStatus */
        switch (replyStatus) {
        case 0: {
            /* success */
            break;
        }
        case 1: {
            /* user exception */
            Ice_ObjectPrx_parseUserException(self, &is);
            return;
        }
        case 2: {
            c_throw("Ice_ObjectDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 3: {
            c_throw("Ice_FacetDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 4: {
            c_throw("Ice_OperationDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 5:
        case 6:
        case 7: {
            /* unkown exception (reason as string) */
            /* FIXME: unmarshall and print reason */
            c_throw("Ice_UnknownException", __FILE__, __LINE__);
            return;
        }}
        
        Ice_InputStream_readInt(&is, &encapsSize);
            Ice_InputStream_readByte(&is, &skipByte);
            Ice_InputStream_readByte(&is, &skipByte);
        }
    
    Ice_ObjectPrx_closeConnection(self);
}

typedef struct st_IString {
    Ice_Object __base;
} st_IString;

typedef st_IString* st_IStringPtr;

typedef Ice_ObjectPrx st_IStringPrx;
typedef Ice_ObjectPrx* st_IStringPrxPtr;

/* forward declaration */
static void
st_IString_methodHandler(st_IStringPtr self,
                         Ice_InputStreamPtr is,
                         Ice_OutputStreamPtr os);

static void
st_IString_init(st_IStringPtr self) {

    trace();
    
    Ice_Object_init((Ice_ObjectPtr)self,
            (Ice_MethodHandlerPtr)st_IString_methodHandler);
}

void
st_IStringI_set(st_IStringPtr self,
                Ice_String v,
                Ice_String sourceAddr) __attribute__((weak));

static void
st_IString_set__dispatch(st_IStringPtr self,
                         Ice_InputStreamPtr is,
                         Ice_OutputStreamPtr os) {
    Ice_String v;
    Ice_String sourceAddr;

    trace();
    
    if (&st_IStringI_set == 0) {
        return;
    }
    Ice_InputStream_readIceString(is, &v);
    
    Ice_InputStream_readIceString(is, &sourceAddr);
    
    st_IStringI_set(self, v, sourceAddr);
}

static void
st_IString_methodHandler(st_IStringPtr self,
                         Ice_InputStreamPtr is,
                         Ice_OutputStreamPtr os) {
    Ice_Int operationSize = 0;
    char operation[MAX_OPERATION_NAME_SIZE];
    byte mode = 0;
    byte contextSize = 0;
    Ice_Int encapSize = 0;
    byte major = 0;
    byte minor = 0;

    trace();
    
    Ptr_check(self);
    Ptr_check(is);
    Ptr_check(os);
    
    Ice_InputStream_readSize(is, &operationSize, false);
    assert(operationSize <= MAX_OPERATION_NAME_SIZE);
    
    Ice_InputStream_readString(is, operation);
    
    /* FIXME: operationMode ignored */
    Ice_InputStream_readByte(is, &mode);
    
    /* FIXME: Context not supported */
    Ice_InputStream_readByte(is, &contextSize);
    assert(contextSize == 0);
    
    Ice_InputStream_readInt(is, &encapSize);
    Ice_InputStream_readByte(is, &major);
    Ice_InputStream_readByte(is, &minor);
    assert(major == 1 && minor == 0);
    assert(encapSize - 6 == (is->size - (is->next - is->data)));
    
    if (strcmp(operation, "set") == 0) {
        st_IString_set__dispatch(self, is, os);
        return;
    }
    
    c_throw("Ice_OperationNotExistException", __FILE__, __LINE__);
}

static void
st_IString_set(st_IStringPrxPtr self,
               Ice_String v,
               Ice_String sourceAddr) {
    Ice_OutputStreamPtr os = &(self->stream);

    trace();
    
    Ptr_check(self);
    
    Ice_ObjectPrx_connect(self);
    Ice_OutputStream_init(os);
    
    /* request header */
    Ice_OutputStream_writeBlob(os, Ice_header, sizeof(Ice_header));
    
    /* request body */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 1);
    }
    if (self->mode != Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 0);
    }
    
    Ice_OutputStream_writeIdentity(os, self->identity);
    Ice_OutputStream_writeString(os, "");
    Ice_OutputStream_writeString(os, "set");
    Ice_OutputStream_writeByte(os, Ice_Normal);
    Ice_OutputStream_writeByte(os, 0);
    
    /* encapsulated params */
    Ice_OutputStream_startWriteEncaps(os);
    Ice_OutputStream_writeIceString(os, v);
    Ice_OutputStream_writeIceString(os, sourceAddr);
    Ice_OutputStream_endWriteEncaps(os);
    Ice_OutputStream_setMessageSize(os);
    
    Ice_ObjectPrx_send(self);
    
    /* parse reply message (only on twoway invocations) */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_InputStream is;
        char expected[10] = {'I', 'c', 'e', 'P', 1, 0, 1, 0, Ice_replyMsg, 0};
        Ice_Int size = 0;
        Ice_Int requestId = 0;
        Ice_Byte replyStatus = 0;
        Ice_Int encapsSize = 0;
        Ice_Byte skipByte = 0;
        
        uint16_t timeout = 5000;
        bool ready = false;
        while (!ready) {
            IcePlugin_Connection_dataReady(&(self->connection), &ready);
            IceUtil_delay(1);
            if (! --timeout) {
                c_throw("Ice_TimeoutException", __FILE__, __LINE__);
                return;
            }
        }
        
        /* reply header */
        Ice_InputStream_init(&is, self->connection.fd, self->connection.epinfo->type);
        if (memcmp(expected, is.next, 10) != 0) {
            c_throw("Ice_ProtocolException", __FILE__, __LINE__);
            return;
        }
        
        /* skip header from input stream */
        is.next += 10;
        Ice_InputStream_readInt(&is, &size);
            if (size > MAX_MESSAGE_SIZE) {
            c_throw("Ice_MemoryLimitException", __FILE__, __LINE__);
            return;
        }
        
        /* reply body */
        Ice_InputStream_readInt(&is, &requestId);
            Ice_InputStream_readByte(&is, &replyStatus);
            
        /* check replyStatus */
        switch (replyStatus) {
        case 0: {
            /* success */
            break;
        }
        case 1: {
            /* user exception */
            Ice_ObjectPrx_parseUserException(self, &is);
            return;
        }
        case 2: {
            c_throw("Ice_ObjectDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 3: {
            c_throw("Ice_FacetDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 4: {
            c_throw("Ice_OperationDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 5:
        case 6:
        case 7: {
            /* unkown exception (reason as string) */
            /* FIXME: unmarshall and print reason */
            c_throw("Ice_UnknownException", __FILE__, __LINE__);
            return;
        }}
        
        Ice_InputStream_readInt(&is, &encapsSize);
            Ice_InputStream_readByte(&is, &skipByte);
            Ice_InputStream_readByte(&is, &skipByte);
        }
    
    Ice_ObjectPrx_closeConnection(self);
}

typedef struct st_Linkable {
    Ice_Object __base;
} st_Linkable;

typedef st_Linkable* st_LinkablePtr;

typedef Ice_ObjectPrx st_LinkablePrx;
typedef Ice_ObjectPrx* st_LinkablePrxPtr;

/* forward declaration */
static void
st_Linkable_methodHandler(st_LinkablePtr self,
                          Ice_InputStreamPtr is,
                          Ice_OutputStreamPtr os);

static void
st_Linkable_init(st_LinkablePtr self) {

    trace();
    
    Ice_Object_init((Ice_ObjectPtr)self,
            (Ice_MethodHandlerPtr)st_Linkable_methodHandler);
}

void
st_LinkableI_linkto(st_LinkablePtr self,
                    Ice_String observerAddr) __attribute__((weak));

static void
st_Linkable_linkto__dispatch(st_LinkablePtr self,
                             Ice_InputStreamPtr is,
                             Ice_OutputStreamPtr os) {
    Ice_String observerAddr;

    trace();
    
    if (&st_LinkableI_linkto == 0) {
        return;
    }
    Ice_InputStream_readIceString(is, &observerAddr);
    
    st_LinkableI_linkto(self, observerAddr);
}

static void
st_Linkable_methodHandler(st_LinkablePtr self,
                          Ice_InputStreamPtr is,
                          Ice_OutputStreamPtr os) {
    Ice_Int operationSize = 0;
    char operation[MAX_OPERATION_NAME_SIZE];
    byte mode = 0;
    byte contextSize = 0;
    Ice_Int encapSize = 0;
    byte major = 0;
    byte minor = 0;

    trace();
    
    Ptr_check(self);
    Ptr_check(is);
    Ptr_check(os);
    
    Ice_InputStream_readSize(is, &operationSize, false);
    assert(operationSize <= MAX_OPERATION_NAME_SIZE);
    
    Ice_InputStream_readString(is, operation);
    
    /* FIXME: operationMode ignored */
    Ice_InputStream_readByte(is, &mode);
    
    /* FIXME: Context not supported */
    Ice_InputStream_readByte(is, &contextSize);
    assert(contextSize == 0);
    
    Ice_InputStream_readInt(is, &encapSize);
    Ice_InputStream_readByte(is, &major);
    Ice_InputStream_readByte(is, &minor);
    assert(major == 1 && minor == 0);
    assert(encapSize - 6 == (is->size - (is->next - is->data)));
    
    if (strcmp(operation, "linkto") == 0) {
        st_Linkable_linkto__dispatch(self, is, os);
        return;
    }
    
    c_throw("Ice_OperationNotExistException", __FILE__, __LINE__);
}

static void
st_Linkable_linkto(st_LinkablePrxPtr self,
                   Ice_String observerAddr) {
    Ice_OutputStreamPtr os = &(self->stream);

    trace();
    
    Ptr_check(self);
    
    Ice_ObjectPrx_connect(self);
    Ice_OutputStream_init(os);
    
    /* request header */
    Ice_OutputStream_writeBlob(os, Ice_header, sizeof(Ice_header));
    
    /* request body */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 1);
    }
    if (self->mode != Ice_ReferenceModeTwoway) {
        Ice_OutputStream_writeInt(os, 0);
    }
    
    Ice_OutputStream_writeIdentity(os, self->identity);
    Ice_OutputStream_writeString(os, "");
    Ice_OutputStream_writeString(os, "linkto");
    Ice_OutputStream_writeByte(os, Ice_Normal);
    Ice_OutputStream_writeByte(os, 0);
    
    /* encapsulated params */
    Ice_OutputStream_startWriteEncaps(os);
    Ice_OutputStream_writeIceString(os, observerAddr);
    Ice_OutputStream_endWriteEncaps(os);
    Ice_OutputStream_setMessageSize(os);
    
    Ice_ObjectPrx_send(self);
    
    /* parse reply message (only on twoway invocations) */
    if (self->mode == Ice_ReferenceModeTwoway) {
        Ice_InputStream is;
        char expected[10] = {'I', 'c', 'e', 'P', 1, 0, 1, 0, Ice_replyMsg, 0};
        Ice_Int size = 0;
        Ice_Int requestId = 0;
        Ice_Byte replyStatus = 0;
        Ice_Int encapsSize = 0;
        Ice_Byte skipByte = 0;
        
        uint16_t timeout = 5000;
        bool ready = false;
        while (!ready) {
            IcePlugin_Connection_dataReady(&(self->connection), &ready);
            IceUtil_delay(1);
            if (! --timeout) {
                c_throw("Ice_TimeoutException", __FILE__, __LINE__);
                return;
            }
        }
        
        /* reply header */
        Ice_InputStream_init(&is, self->connection.fd, self->connection.epinfo->type);
        if (memcmp(expected, is.next, 10) != 0) {
            c_throw("Ice_ProtocolException", __FILE__, __LINE__);
            return;
        }
        
        /* skip header from input stream */
        is.next += 10;
        Ice_InputStream_readInt(&is, &size);
            if (size > MAX_MESSAGE_SIZE) {
            c_throw("Ice_MemoryLimitException", __FILE__, __LINE__);
            return;
        }
        
        /* reply body */
        Ice_InputStream_readInt(&is, &requestId);
            Ice_InputStream_readByte(&is, &replyStatus);
            
        /* check replyStatus */
        switch (replyStatus) {
        case 0: {
            /* success */
            break;
        }
        case 1: {
            /* user exception */
            Ice_ObjectPrx_parseUserException(self, &is);
            return;
        }
        case 2: {
            c_throw("Ice_ObjectDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 3: {
            c_throw("Ice_FacetDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 4: {
            c_throw("Ice_OperationDoesNotExist", __FILE__, __LINE__);
            return;
        }
        case 5:
        case 6:
        case 7: {
            /* unkown exception (reason as string) */
            /* FIXME: unmarshall and print reason */
            c_throw("Ice_UnknownException", __FILE__, __LINE__);
            return;
        }}
        
        Ice_InputStream_readInt(&is, &encapsSize);
            Ice_InputStream_readByte(&is, &skipByte);
            Ice_InputStream_readByte(&is, &skipByte);
        }
    
    Ice_ObjectPrx_closeConnection(self);
}

