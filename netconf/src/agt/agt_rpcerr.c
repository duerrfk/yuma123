/*  FILE: agt_rpcerr.c

   NETCONF Protocol Operations: <rpc-error> Agent Side Support

*********************************************************************
*                                                                   *
*                  C H A N G E   H I S T O R Y                      *
*                                                                   *
*********************************************************************

date         init     comment
----------------------------------------------------------------------
06mar06      abb      begun; split from agt_rpc.c


*********************************************************************
*                                                                   *
*                     I N C L U D E    F I L E S                    *
*                                                                   *
*********************************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <memory.h>

#ifndef _H_procdefs
#include  "procdefs.h"
#endif

#ifndef _H_agt_rpc
#include  "agt_rpc.h"
#endif

#ifndef _H_agt_rpcerr
#include  "agt_rpcerr.h"
#endif

#ifndef _H_cfg
#include  "cfg.h"
#endif

#ifndef _H_def_reg
#include  "def_reg.h"
#endif

#ifndef _H_ncx
#include  "ncx.h"
#endif

#ifndef _H_ncxconst
#include  "ncxconst.h"
#endif

#ifndef _H_rpc
#include  "rpc.h"
#endif

#ifndef _H_rpc_err
#include  "rpc_err.h"
#endif

#ifndef _H_status
#include  "status.h"
#endif

#ifndef _H_val
#include  "val.h"
#endif

#ifndef _H_val_util
#include  "val_util.h"
#endif

#ifndef _H_xmlns
#include  "xmlns.h"
#endif

#ifndef _H_xml_util
#include  "xml_util.h"
#endif

/********************************************************************
*                                                                   *
*                       C O N S T A N T S                           *
*                                                                   *
*********************************************************************/

#define AGT_RPCERR_DEBUG 1

/********************************************************************
*                                                                   *
*                           T Y P E S                               *
*                                                                   *
*********************************************************************/
    

/********************************************************************
*                                                                   *
*                       V A R I A B L E S			    *
*                                                                   *
*********************************************************************/


/********************************************************************
* FUNCTION get_rpcerr
*
* Translate the status_t to a rpc_err_t 
*
* INPUTS:
*   intres == internal status_t error code
*   isel == TRUE if this error is for an element
*        == FALSE if this error is for an attribute
*
* OUTPUTS:
*  *errsev == error severity (error or warning)
*
* RETURNS:
*   rpc error id for the specified internal error id
*********************************************************************/
static rpc_err_t
    get_rpcerr (status_t intres,
		boolean  isel,
		rpc_err_sev_t *errsev)
{

    if (intres >= ERR_WARN_BASE) {
	*errsev = RPC_ERR_SEV_WARNING;
	return RPC_ERR_NONE;
    }

    *errsev = RPC_ERR_SEV_ERROR;

    
    /* translate the internal NCX error code to a netconf code */
    switch (intres) {
    case NO_ERR:
	*errsev = RPC_ERR_SEV_NONE;
	return RPC_ERR_NONE;
    case ERR_END_OF_FILE:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_INTERNAL_PTR:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_INTERNAL_MEM:
	return RPC_ERR_RESOURCE_DENIED;
    case ERR_INTERNAL_VAL:
    case ERR_INTERNAL_BUFF:
    case ERR_INTERNAL_QDEL:
    case ERR_INTERNAL_INIT_SEQ:
    case ERR_QNODE_NOT_HDR:
    case ERR_QNODE_NOT_DATA:
    case ERR_BAD_QLINK:
    case ERR_Q_ALREADY:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_TOO_MANY_ENTRIES:
	return RPC_ERR_RESOURCE_DENIED;
    case ERR_XML2_FAILED:
    case ERR_FIL_OPEN:
    case ERR_FIL_READ:
    case ERR_FIL_CLOSE:
    case ERR_FIL_WRITE:
    case ERR_FIL_CHDIR:
    case ERR_FIL_STAT:
    case ERR_BUFF_OVFL:
    case ERR_FIL_DELETE:
    case ERR_FIL_SETPOS:
    case ERR_DB_CONNECT_FAILED:
    case ERR_DB_ENTRY_EXISTS:
    case ERR_DB_NOT_FOUND:
    case ERR_DB_QUERY_FAILED:
    case ERR_DB_DELETE_FAILED:
    case ERR_DB_WRONG_CKSUM:
    case ERR_DB_WRONG_TAGTYPE:
    case ERR_DB_READ_FAILED:
    case ERR_DB_WRITE_FAILED:
    case ERR_DB_INIT_FAILED:
    case ERR_TR_BEEP_INIT:
    case ERR_TR_BEEP_NC_INIT:
    case ERR_XML_READER_INTERNAL:
    case ERR_OPEN_DIR_FAILED:
    case ERR_READ_DIR_FAILED:
    case ERR_NO_CFGFILE:
    case ERR_NO_SRCFILE:
    case ERR_PARSPOST_RD_INPUT:
    case ERR_FIL_BAD_DRIVE:
    case ERR_FIL_BAD_PATH:
    case ERR_FIL_BAD_FILENAME:
    case ERR_DUP_VALPAIR:
    case ERR_PAGE_NOT_HANDLED:
    case ERR_PAGE_ACCESS_DENIED:
    case ERR_MISSING_FORM_PARAMS:
    case ERR_FORM_STATE:
    case ERR_DUP_NS:
    case ERR_XML_READER_START_FAILED:
    case ERR_XML_READER_READ:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_XML_READER_NODETYP:
	return (isel)  ? RPC_ERR_UNKNOWN_ELEMENT 
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_XML_READER_NULLNAME:
    case ERR_XML_READER_NULLVAL:
	return (isel) ? RPC_ERR_BAD_ELEMENT
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_XML_READER_WRONGNAME:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_XML_READER_WRONGVAL:
	return (isel) ? RPC_ERR_BAD_ELEMENT 
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_XML_READER_WRONGEL:
	return RPC_ERR_UNKNOWN_ELEMENT;	
    case ERR_XML_READER_EXTRANODES:
	return RPC_ERR_UNKNOWN_ELEMENT;	
    case ERR_XML_READER_EOF:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_WRONG_LEN:
	return (isel) ? RPC_ERR_BAD_ELEMENT
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_ENTRY_EXISTS:
    case ERR_NCX_DUP_ENTRY:
	return RPC_ERR_DATA_EXISTS;
    case ERR_NCX_NOT_FOUND:
    case ERR_NCX_MISSING_FILE:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_UNKNOWN_PARM:
	return RPC_ERR_UNKNOWN_ELEMENT;
    case ERR_NCX_INVALID_NAME:
	return (isel) ? RPC_ERR_BAD_ELEMENT
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_UNKNOWN_NS:
	return RPC_ERR_UNKNOWN_NAMESPACE;
    case ERR_NCX_WRONG_NS:
	return RPC_ERR_UNKNOWN_NAMESPACE;
    case ERR_NCX_WRONG_TYPE:
	return (isel) ? RPC_ERR_BAD_ELEMENT
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_WRONG_VAL:
	return (isel) ? RPC_ERR_BAD_ELEMENT
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_MISSING_PARM:
	return RPC_ERR_DATA_MISSING;
    case ERR_NCX_EXTRA_PARM:
	return RPC_ERR_UNKNOWN_ELEMENT;
    case ERR_NCX_EMPTY_VAL:
	return (isel) ? RPC_ERR_BAD_ELEMENT 
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_MOD_NOT_FOUND:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_LEN_EXCEEDED:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_INVALID_TOKEN:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_UNENDED_QSTRING:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_READ_FAILED:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_INVALID_NUM:
    case ERR_NCX_INVALID_HEXNUM:
    case ERR_NCX_INVALID_REALNUM:
	return (isel) ? RPC_ERR_BAD_ELEMENT 
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_EOF:
	return RPC_ERR_DATA_MISSING;
    case ERR_NCX_WRONG_TKTYPE:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_WRONG_TKVAL:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_BUFF_SHORT:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_INVALID_RANGE:
    case ERR_NCX_OVERLAP_RANGE:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_DEF_NOT_FOUND:
    case ERR_NCX_DEFSEG_NOT_FOUND:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_TYPE_NOT_INDEX:
	return (isel) ? RPC_ERR_BAD_ELEMENT 
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_INDEX_TYPE_NOT_FOUND:
    case ERR_NCX_TYPE_NOT_MDATA:
    case ERR_NCX_MDATA_NOT_ALLOWED:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_TOP_NOT_FOUND:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;

    /*** match netconf errors here ***/
    case ERR_NCX_IN_USE:
	return RPC_ERR_IN_USE;
    case ERR_NCX_INVALID_VALUE:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_TOO_BIG:
	return RPC_ERR_TOO_BIG;
    case ERR_NCX_MISSING_ATTRIBUTE:
	return RPC_ERR_MISSING_ATTRIBUTE;
    case ERR_NCX_BAD_ATTRIBUTE:
	return RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_UNKNOWN_ATTRIBUTE:
	return RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_MISSING_ELEMENT:
	return RPC_ERR_MISSING_ELEMENT;
    case ERR_NCX_BAD_ELEMENT:
	return RPC_ERR_BAD_ELEMENT;
    case ERR_NCX_UNKNOWN_ELEMENT:
	return RPC_ERR_UNKNOWN_ELEMENT;
    case ERR_NCX_UNKNOWN_NAMESPACE:
	return RPC_ERR_UNKNOWN_NAMESPACE;
    case ERR_NCX_ACCESS_DENIED:
	return RPC_ERR_ACCESS_DENIED;
    case ERR_NCX_LOCK_DENIED:
	return RPC_ERR_LOCK_DENIED;
    case ERR_NCX_RESOURCE_DENIED:
	return RPC_ERR_RESOURCE_DENIED;
    case ERR_NCX_ROLLBACK_FAILED:
	return RPC_ERR_ROLLBACK_FAILED;
    case ERR_NCX_DATA_EXISTS:
	return RPC_ERR_DATA_EXISTS;
    case ERR_NCX_DATA_MISSING:
	return RPC_ERR_DATA_MISSING;
    case ERR_NCX_OPERATION_NOT_SUPPORTED:
	return RPC_ERR_OPERATION_NOT_SUPPORTED;
    case ERR_NCX_OPERATION_FAILED:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_PARTIAL_OPERATION:
	return RPC_ERR_PARTIAL_OPERATION;

    /* netconf error extensions */
    case ERR_NCX_WRONG_NAMESPACE:
	return RPC_ERR_UNKNOWN_NAMESPACE;
    case ERR_NCX_WRONG_NODEDEPTH:
	return RPC_ERR_BAD_ELEMENT;
    case ERR_NCX_WRONG_OWNER:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_WRONG_ELEMENT:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_WRONG_ORDER:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_EXTRA_NODE:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_WRONG_NODETYP:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_WRONG_NODETYP_SIM:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_WRONG_NODETYP_CPX:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_WRONG_DATATYP:
    case ERR_NCX_WRONG_DATAVAL:
    case ERR_NCX_NUMLEN_TOOBIG:
    case ERR_NCX_WRONG_NUMTYP:
    case ERR_NCX_EXTRA_ENUMCH:
    case ERR_NCX_EXTRA_LISTSTR:
	return (isel) ? RPC_ERR_BAD_ELEMENT 
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_NOT_IN_RANGE:
    case ERR_NCX_VAL_NOTINSET:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_UNKNOWN_OBJECT:
	return RPC_ERR_UNKNOWN_ELEMENT;
    case ERR_NCX_EXTRA_PARMINST:
    case ERR_NCX_EXTRA_CHOICE:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT 
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_MISSING_CHOICE:
	return RPC_ERR_DATA_MISSING;
    case ERR_NCX_CFG_STATE:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_UNKNOWN_APP:
	return RPC_ERR_UNKNOWN_ELEMENT;
    case ERR_NCX_UNKNOWN_TYPE:
	return (isel) ? RPC_ERR_UNKNOWN_ELEMENT 
	    : RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_NO_ACCESS_ACL:
	return RPC_ERR_ACCESS_DENIED;
    case ERR_NCX_NO_ACCESS_LOCK:
	return RPC_ERR_IN_USE;
    case ERR_NCX_NO_ACCESS_STATE:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_NO_ACCESS_MAX:
	return RPC_ERR_ACCESS_DENIED;
    case ERR_NCX_WRONG_INDEX_TYPE:
	return RPC_ERR_BAD_ELEMENT;
    case ERR_NCX_WRONG_INSTANCE_TYPE:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_MISSING_INDEX:
	return RPC_ERR_DATA_MISSING;
    case ERR_NCX_CFG_NOT_FOUND:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_EXTRA_ATTR:
	return RPC_ERR_UNKNOWN_ATTRIBUTE;
    case ERR_NCX_MISSING_ATTR:
	return RPC_ERR_MISSING_ATTRIBUTE;
    case ERR_NCX_MISSING_VAL_INST:
	return RPC_ERR_DATA_MISSING;
    case ERR_NCX_EXTRA_VAL_INST:
	return RPC_ERR_UNKNOWN_ELEMENT;
    case ERR_NCX_NOT_WRITABLE:
	return RPC_ERR_ACCESS_DENIED;
    case ERR_NCX_INVALID_PATTERN:
	return (isel) ? RPC_ERR_BAD_ELEMENT 
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_WRONG_VERSION:
	return (isel) ? RPC_ERR_BAD_ELEMENT 
	    : RPC_ERR_BAD_ATTRIBUTE;
    case ERR_NCX_CONNECT_FAILED:
    case ERR_NCX_SESSION_FAILED:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_UNKNOWN_HOST:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_AUTH_FAILED:
	return RPC_ERR_ACCESS_DENIED;
    case ERR_NCX_UNENDED_COMMENT:
    case ERR_NCX_INVALID_CONCAT:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_IMP_NOT_FOUND:
    case ERR_NCX_MISSING_TYPE:
    case ERR_NCX_RESTRICT_NOT_ALLOWED:
    case ERR_NCX_REFINE_NOT_ALLOWED:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_DEF_LOOP:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_DEFCHOICE_NOT_OPTIONAL:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_IMPORT_LOOP:
    case ERR_NCX_INCLUDE_LOOP:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_EXP_MODULE:
    case ERR_NCX_EXP_SUBMODULE:
    case ERR_NCX_PREFIX_NOT_FOUND:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_IMPORT_ERRORS:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_PATTERN_FAILED:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_INVALID_TYPE_CHANGE:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_MANDATORY_NOT_ALLOWED:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_UNIQUE_TEST_FAILED:
	return RPC_ERR_OPERATION_FAILED;   /* 13.1 */
    case ERR_NCX_MAX_ELEMS_VIOLATION:
	return RPC_ERR_OPERATION_FAILED;   /* 13.2 */
    case ERR_NCX_MIN_ELEMS_VIOLATION:
	return RPC_ERR_OPERATION_FAILED;   /* 13.3 */
    case ERR_NCX_MUST_TEST_FAILED:
	return RPC_ERR_OPERATION_FAILED;   /* 13.4 */
    case ERR_NCX_DATA_REST_VIOLATION:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_INSERT_MISSING_INSTANCE:
	return RPC_ERR_BAD_ATTRIBUTE;      /* 13.5 */
    case ERR_NCX_NOT_CONFIG:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_INVALID_CONDITIONAL:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_USING_OBSOLETE:
    case ERR_NCX_INVALID_AUGTARGET:
    case ERR_NCX_DUP_REFINE_STMT:
    case ERR_NCX_INVALID_DEV_STMT:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_INVALID_XPATH_EXPR:
    case ERR_NCX_INVALID_INSTANCEID:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_MISSING_INSTANCE:  /* 13.n */
	return RPC_ERR_DATA_MISSING;
    case ERR_NCX_UNEXPECTED_INSERT_ATTRS:
	return RPC_ERR_OPERATION_FAILED;
    case ERR_NCX_INVALID_UNIQUE_NODE:
    case ERR_NCX_INVALID_DUP_IMPORT:
    case ERR_NCX_INVALID_DUP_INCLUDE:
    case ERR_NCX_AMBIGUOUS_CMD:
    case ERR_NCX_UNKNOWN_MODULE:
    case ERR_NCX_UNKNOWN_VERSION:
    case ERR_NCX_VALUE_NOT_SUPPORTED:
    case ERR_NCX_LEAFREF_LOOP:
    case ERR_NCX_VAR_NOT_FOUND:
    case ERR_NCX_VAR_READ_ONLY:
    case ERR_NCX_DEC64_BASEOVFL:
    case ERR_NCX_DEC64_FRACOVFL:
	return RPC_ERR_INVALID_VALUE;
    case ERR_NCX_RPC_WHEN_FAILED:
	return RPC_ERR_UNKNOWN_ELEMENT;

    /* user warnings start at 400 and do not need to be listed here */
    default:
	return RPC_ERR_OPERATION_FAILED;	
    }
    /*NOTREACHED*/

} /* get_rpcerr */


/********************************************************************
* FUNCTION start_err
*
* Malloc an rpc_err_rec_t and set up the initial fields
*
* INPUTS:
*   layer == protocol layer where the error occurred
*   interr == internal error code
*   rpcerr == rpc_err_t to use
*   errsev == error severity
*   apptag == the error-app-tag that should be generated.
*   error_path == malloced error_path string that will be saved
*   error_msg == error-msg to use instead of default message
*                that might be be generated.  This string is assumed
*                to be a malloced string that will be saved in the
*                rpc_err_rec_t struct and freed when the
*                rpc_err_clean_record function is called for
*                this record.  
*             == NULL if the default msg text should be used instead
*   error_lang == a 'lang' value string such as EN, JP, or FR 
*              == NULL if the default (EN) should be used
*              This parameter is ignored if error_msg == NULL
*
* RETURNS:
*   pointer to allocated and filled in rpc_err_rec_t struct
*     ready to add to the msg->rpc_errQ
*   NULL if a record could not be allocated or not enough
*     val;id info in the parameters 
*********************************************************************/
static rpc_err_rec_t *
    start_err (ncx_layer_t layer,
	       status_t    interr,
	       rpc_err_t   rpcerr,
	       rpc_err_sev_t   errsev,
	       const xmlChar *apptag,
	       xmlChar *error_path,
	       xmlChar *error_msg,
	       const xmlChar *error_lang)
{
    rpc_err_rec_t  *err;

    /* get a new error record */
    err = rpc_err_new_record();
    if (!err) {
	return NULL;
    }

    /* setup the return record */
    err->error_res = interr;
    err->error_id = rpcerr;
    err->error_type = layer;
    err->error_severity = errsev;
    err->error_tag = rpc_err_get_errtag(rpcerr);
    err->error_app_tag = apptag;
    err->error_path = error_path;
    err->error_message = error_msg;
    err->error_message_lang = (error_lang) ? 
	error_lang : NCX_DEF_LANG;

    return err;

}  /* start_err */


/********************************************************************
* FUNCTION add_base_vars
*
* Malloc val_value_t structs, fill them in and add them to
* the err->error_info Q, for the basic vars that are
* generated for specific error-tag values.
*
* INPUTS:
*   err == rpc_err_rec_t in progress
*   rpcerr == error-tag internal error code
*   errnode == XML node that caused the error (or NULL)
*   badval == bad_value string (or NULL)
*   badns == bad-namespace string (or NULL)
*   badnsid1 == bad NSID #1
*   badnsid2 == bad NSID #2
*   errparm1 == void * to the 1st extra parameter to use (per errcode)
*           == NULL if not use
*   errparm2 == void * to the 2nd extra parameter to use (per errcode)
*           == NULL if not used
*   !!! errparm3 removed !!!
*   errparm4 == void * to the 4th extra parameter to use (per errcode)
*           == NULL if not used

* RETURNS:
*   status
*********************************************************************/
static status_t
    add_base_vars (rpc_err_rec_t  *err,
		   rpc_err_t  rpcerr,
		   const xml_node_t *errnode,
		   const xmlChar *badval,
		   const xmlChar *badns,
		   xmlns_id_t  badnsid1,
		   xmlns_id_t  badnsid2,
		   const void *errparm1,
		   const void *errparm2,
		   const void *errparm4)
{
    rpc_err_info_t       *errinfo;
    const xmlChar        *badel;
    const ses_cb_t       *badscb;
    ses_id_t              sesid;
    boolean               attrerr;
    xmlns_id_t            ncid, ncxid, badid;

    /* setup local vars */
    ncid = xmlns_nc_id();
    ncxid = xmlns_ncx_id();
    attrerr = FALSE;

    /* figure out the required error-info */
    switch (rpcerr) {
    case RPC_ERR_UNKNOWN_ATTRIBUTE:
    case RPC_ERR_MISSING_ATTRIBUTE:
    case RPC_ERR_BAD_ATTRIBUTE:
	/* badnsid1 == error attribute namespace ID
	 * errparm2 == error attribute name
	 */
	if (errparm2) {
	    badid = badnsid1;
	    badel = (const xmlChar *)errparm2;

	    errinfo = rpc_err_new_info();
	    if (!errinfo) {
		return ERR_INTERNAL_MEM;
	    }
	
	    /* generate bad-attribute value */
	    errinfo->name_nsid = ncid;
	    errinfo->name = NCX_EL_BAD_ATTRIBUTE;
	    errinfo->val_btype = NCX_BT_STRING;
	    errinfo->val_nsid = badid;
	    if (badns && badid == xmlns_inv_id()) {
		errinfo->badns = xml_strdup(badns);
	    }
	    errinfo->dval = xml_strdup(badel);
	    if (!errinfo->dval) {
		rpc_err_free_info(errinfo);
		return ERR_INTERNAL_MEM;
	    }
	    errinfo->v.strval = errinfo->dval;
	    errinfo->isqname = TRUE;
	    dlq_enque(errinfo, &err->error_info);
	} /* else internal error already recorded */
	attrerr = TRUE;
	/* fall through */
    case RPC_ERR_UNKNOWN_ELEMENT:
    case RPC_ERR_MISSING_ELEMENT:
    case RPC_ERR_BAD_ELEMENT:	
    case RPC_ERR_UNKNOWN_NAMESPACE:
	/* check mandatory param for this rpcerr */
	if (errnode) {
	    badid = errnode->nsid;
	    badel = errnode->elname;
	} else if (attrerr) {
	    badid = badnsid2;
	    badel = (const xmlChar *)errparm4;
	} else {
	    badid = badnsid1;
	    badel = (const xmlChar *)errparm2;
	}

	/* generate bad-element value */
	if (badel) {
	    errinfo = rpc_err_new_info();
	    if (!errinfo) {
		return ERR_INTERNAL_MEM;
	    }
	    errinfo->name_nsid = ncid;
	    errinfo->name = NCX_EL_BAD_ELEMENT;
	    errinfo->val_btype = NCX_BT_STRING;

	    errinfo->val_nsid = badid;
	    if (badns && badid == xmlns_inv_id()) {
		errinfo->badns = xml_strdup(badns);
	    }

	    errinfo->dval = xml_strdup(badel);
	    if (!errinfo->dval) {
		rpc_err_free_info(errinfo);
		return ERR_INTERNAL_MEM;
	    }
	    errinfo->v.strval = errinfo->dval;
	    errinfo->isqname = TRUE;
	    dlq_enque(errinfo, &err->error_info);
	} else {
	    SET_ERROR(ERR_INTERNAL_VAL);
	}

	if (rpcerr == RPC_ERR_UNKNOWN_NAMESPACE) {
	    if (badns) {
		/* generate bad-namespace element */
		errinfo = rpc_err_new_info();
		if (!errinfo) {
		    return ERR_INTERNAL_MEM;
		}
		errinfo->dval = xml_strdup(badns);
		if (!errinfo->dval) {
		    rpc_err_free_info(errinfo);
		    return ERR_INTERNAL_MEM;
		}
		errinfo->name_nsid = ncid;
		errinfo->name = NCX_EL_BAD_NAMESPACE;
		errinfo->val_btype = NCX_BT_STRING;
		errinfo->val_nsid = 0;
		errinfo->v.strval = errinfo->dval;
		dlq_enque(errinfo, &err->error_info);
	    } else {
		SET_ERROR(ERR_INTERNAL_VAL);
	    }
	}
	break;
    case RPC_ERR_LOCK_DENIED:
	/* generate session-id value */
	badscb = (const ses_cb_t *)errparm1;
	sesid = badscb->sid;

	errinfo = rpc_err_new_info();
	if (!errinfo) {
	    return ERR_INTERNAL_MEM;
	}
	errinfo->name_nsid = ncid;
	errinfo->name = NCX_EL_SESSION_ID;
	errinfo->val_btype = NCX_BT_UINT32;
	errinfo->val_nsid = 0;
	errinfo->v.numval.u = (uint32)sesid;
	dlq_enque(errinfo, &err->error_info);
	break;
    case RPC_ERR_DATA_MISSING:
	if (errparm2) {
	    /* expecting the obj_template_t of the missing choice */
	    badel = (const xmlChar *)errparm2;
	    
	    errinfo = rpc_err_new_info();
	    if (!errinfo) {
		return ERR_INTERNAL_MEM;
	    }
	    errinfo->dval = xml_strdup(badel);
	    if (!errinfo->dval) {
		rpc_err_free_info(errinfo);
		return ERR_INTERNAL_MEM;
	    }
	    errinfo->name_nsid = xmlns_yang_id();
	    errinfo->name = (const xmlChar *)"missing-choice";
	    errinfo->val_btype = NCX_BT_STRING;
	    errinfo->val_nsid = 0;
	    errinfo->v.strval = errinfo->dval;
	    dlq_enque(errinfo, &err->error_info);
	} else {
	    SET_ERROR(ERR_INTERNAL_VAL);
	}
	break;
    default:
	;   /* all other rpc-err_t enums handled elsewhere */
    } 

    /* generate NCX extension bad-value */
    if (badval) {
	errinfo = rpc_err_new_info();
	if (!errinfo) {
	    return ERR_INTERNAL_MEM;
	}
	errinfo->dval = xml_strdup(badval);
	if (!errinfo->dval) {
	    rpc_err_free_info(errinfo);
	    return ERR_INTERNAL_MEM;
	}

	errinfo->name_nsid = ncxid;
	errinfo->name = NCX_EL_BAD_VALUE;
	errinfo->val_btype = NCX_BT_STRING;
	errinfo->val_nsid = 0;
	errinfo->v.strval = errinfo->dval;
	dlq_enque(errinfo, &err->error_info);
    }

    return NO_ERR;

}  /* add_base_vars */


/**************** E X T E R N A L   F U N C T I O N S  ***************/



/********************************************************************
* FUNCTION agt_rpc_gen_error
*
* Generate an internal <rpc-error> record for an element
*
* INPUTS:
*   layer == protocol layer where the error occurred
*   interr == internal error code
*             if NO_ERR than use the rpcerr only
*   errnode == XML node where error occurred
*           == NULL then there is no valid XML node (maybe the error!)
*   parmtyp == type of node contained in error_parm
*   error_parm == pointer to the extra parameter expected for
*                this type of error.  
*
*                == (void *)pointer to session_id for lock-denied errors
*                == (void *) pointer to the bad-value string to use
*                   for some other errors
*
*  error_path == malloced string of the value (or type, etc.) instance
*                ID string in NCX_IFMT_XPATH format; this will be added
*                to the rpc_err_rec_t and freed later
*             == NULL if not available
*
* RETURNS:
*   pointer to allocated and filled in rpc_err_rec_t struct
*     ready to add to the msg->rpc_errQ
*   NULL if a record could not be allocated or not enough
*     val;id info in the parameters 
*********************************************************************/
rpc_err_rec_t *
    agt_rpcerr_gen_error (ncx_layer_t layer,
			  status_t   interr,
			  const xml_node_t *errnode,
			  ncx_node_t  parmtyp,
			  const void *error_parm,
			  xmlChar *error_path)
{
    return agt_rpcerr_gen_error_errinfo(layer, 
					interr, 
					errnode, 
					parmtyp, 
					error_parm, 
					error_path, 
					NULL);

} /* agt_rpcerr_gen_error */


/********************************************************************
* FUNCTION agt_rpc_gen_error_errinfo
*
* Generate an internal <rpc-error> record for an element
*
* INPUTS:
*   layer == protocol layer where the error occurred
*   interr == internal error code
*             if NO_ERR than use the rpcerr only
*   errnode == XML node where error occurred
*           == NULL then there is no valid XML node (maybe the error!)
*   parmtyp == type of node contained in error_parm
*   error_parm == pointer to the extra parameter expected for
*                this type of error.  
*
*                == (void *)pointer to session_id for lock-denied errors
*                == (void *) pointer to the bad-value string to use
*                   for some other errors
*
*  error_path == malloced string of the value (or type, etc.) instance
*                ID string in NCX_IFMT_XPATH format; this will be added
*                to the rpc_err_rec_t and freed later
*             == NULL if not available
*  errinfo == error info struct to use for whatever fields are set
*
* RETURNS:
*   pointer to allocated and filled in rpc_err_rec_t struct
*     ready to add to the msg->rpc_errQ
*   NULL if a record could not be allocated or not enough
*     val;id info in the parameters 
*********************************************************************/
rpc_err_rec_t *
    agt_rpcerr_gen_error_errinfo (ncx_layer_t layer,
				  status_t   interr,
				  const xml_node_t *errnode,
				  ncx_node_t  parmtyp,
				  const void *error_parm,
				  xmlChar *error_path,
				  const ncx_errinfo_t *errinfo)
{
    rpc_err_rec_t            *err;
    const obj_template_t     *parm, *in, *obj;
    const xmlns_qname_t      *qname;
    const val_value_t        *valparm;
    xmlChar                  *error_msg;
    const xmlChar            *badval, *badns, *msg, *apptag;
    const void               *err1, *err2, *err4;
    rpc_err_t                 rpcerr;
    status_t                  res;
    rpc_err_sev_t             errsev;
    xmlns_id_t                badnsid1, badnsid2;

    qname = NULL;
    badval = NULL;
    badns = NULL;
    err1 = NULL;
    err2 = NULL;
    err4 = NULL;
    badnsid1 = 0;
    badnsid2 = 0;

    switch (interr) {
    case ERR_NCX_MISSING_PARM:
    case ERR_NCX_EXTRA_CHOICE:
    case ERR_NCX_MISSING_CHOICE:
	if (!error_parm) {
	    SET_ERROR(ERR_INTERNAL_PTR);
	} else {
	    switch (parmtyp) {
	    case NCX_NT_OBJ:
		parm = (const obj_template_t *)error_parm;
		if (parm) {
		    badnsid1 = obj_get_nsid(parm);
		    err2 = (const void *)obj_get_name(parm);
		}
		break;
	    case NCX_NT_VAL:
		valparm = (const val_value_t *)error_parm;
		if (valparm) {
		    badnsid1 = val_get_nsid(valparm);
		    err2 = (const void *)valparm->name;
		}
		break;
	    case NCX_NT_STRING:
		badnsid1 = 0;
		err2 = (const void *)error_parm;
		break;
	    default:
		SET_ERROR(ERR_INTERNAL_VAL);
	    }
	}
	break;
    case ERR_NCX_LOCK_DENIED:
	if (!error_parm || parmtyp != NCX_NT_CFG) {
	    SET_ERROR(ERR_INTERNAL_VAL);
	} else {
	    err1 = error_parm;
	}
	break;
    case ERR_NCX_MISSING_INDEX:
	if (!error_parm || parmtyp != NCX_NT_OBJ) {
	    SET_ERROR(ERR_INTERNAL_VAL);
	} else {
	    in = (const obj_template_t *)error_parm;
	    if (in) {
		badnsid1 = obj_get_nsid(in);
		err2 = (const void *)obj_get_name(in);
	    }
	}
    case ERR_NCX_DEF_NOT_FOUND:
	if (parmtyp == NCX_NT_STRING && error_parm) {
	    err1 = error_parm;
	}
	break;
    case ERR_NCX_MISSING_ATTR:
    case ERR_NCX_EXTRA_ATTR:
    case ERR_NCX_MISSING_VAL_INST:
    case ERR_NCX_EXTRA_VAL_INST:
	/* this hack is needed because this error is generated
	 * after the xml_attr_t record is gone
	 *
	 * First set the bad-attribute NS and name
	 */
	if (error_parm) {
	    if (parmtyp == NCX_NT_QNAME) {
		qname = (const xmlns_qname_t *)error_parm;
		badnsid1 = qname->nsid;
		err2 = (const void *)qname->name;
	    } else if (parmtyp == NCX_NT_OBJ) {
		obj = (const obj_template_t *)error_parm;
		badnsid1 = obj_get_nsid(obj);
		err2 = (const void *)obj_get_name(obj);
	    } else if (parmtyp == NCX_NT_STRING) {
		badnsid1 = 0;
		err2 = (const void *)error_parm;
	    } else {
		SET_ERROR(ERR_INTERNAL_VAL);
	    }
	} else {
	    SET_ERROR(ERR_INTERNAL_VAL);
	}

	/* hack: borrow the errnode pointer to use as a string 
	 * for the bad-element name 
	 */
	if (errnode) {
	    badnsid2 = qname->nsid;
	    err4 = (const void *)errnode;
	    /* make sure add_base_vars doesn't use as an xml_node_t */
	    errnode = NULL;  
	}
	break;
    default:
	break;
    }

    rpcerr = get_rpcerr(interr, TRUE, &errsev); 

    /* generate a default error message, and continue anyway
     * if the xml_strdup fails with a malloc error
     */
    if (errinfo && errinfo->error_message) {
	msg = errinfo->error_message;
    } else {
	msg = (const xmlChar *)get_error_string(interr);
    }
    error_msg = (msg) ? xml_strdup(msg) : NULL;

    if (errinfo && errinfo->error_app_tag) {
	apptag = errinfo->error_app_tag;
    } else {
	switch (interr) {
	case ERR_NCX_UNIQUE_TEST_FAILED:
	    apptag = (const xmlChar *)"data-not-unique"; /* 13.1 */
	    break;
	case ERR_NCX_MAX_ELEMS_VIOLATION:
	    apptag = (const xmlChar *)"too-many-elements"; /* 13.2 */
	    break;
	case ERR_NCX_MIN_ELEMS_VIOLATION:
	    apptag = (const xmlChar *)"too-few-elements";  /* 13.3 */
	    break;
	case ERR_NCX_NOT_IN_RANGE:
	    apptag = (const xmlChar *)"not-in-range";
	    break;
	case ERR_NCX_VAL_NOTINSET:
	    apptag = (const xmlChar *)"not-in-value-set"; 
	    break;
	case ERR_NCX_PATTERN_FAILED:
	    apptag = (const xmlChar *)"pattern-test-failed";
	    break;
	case ERR_NCX_DATA_REST_VIOLATION:
	    apptag = (const xmlChar *)"data-restriction-violation";
	    break;
	case ERR_NCX_MUST_TEST_FAILED:
	    apptag = (const xmlChar *)"must-violation";  /* 13.4 */
	    break;
	case ERR_NCX_MISSING_INSTANCE:
	    apptag = (const xmlChar *)"instance-required"; /* 13.n */
	    break;
	case ERR_NCX_INSERT_MISSING_INSTANCE:
	    apptag = (const xmlChar *)"missing-instance"; /* 13.5 */
	    break;
	case ERR_NCX_MISSING_CHOICE:
	    apptag = (const xmlChar *)"missing-choice"; /* 13.n */
	    break;
	default:
	    apptag = NULL;
	}
    }

    /* get a new error record */
    err = start_err(layer, 
		    interr, 
		    rpcerr, 
		    errsev, 
		    apptag, 
		    error_path, 
		    error_msg, 
		    NULL);
    if (!err) {
	if (error_msg) {
	    m__free(error_msg);
	}
	return NULL;
    }

    /* figure out the required error-info 
     * It is possible for an attribute error to be recorded
     * through this function due to errors detected after
     * the xml_node_t and xml_attr_t structs have been discarded
     */
    switch (rpcerr) {
    case RPC_ERR_MISSING_ELEMENT:
	break;
    case RPC_ERR_BAD_ELEMENT:
	if (parmtyp==NCX_NT_STRING) {
	    badval = (const xmlChar *)error_parm;
	}
	break;
    case RPC_ERR_UNKNOWN_ELEMENT:
	break;
    case RPC_ERR_UNKNOWN_NAMESPACE:
	if (parmtyp==NCX_NT_STRING) {
	    badns = (const xmlChar *)error_parm;
	}
	break;
    case RPC_ERR_LOCK_DENIED:
    case RPC_ERR_MISSING_ATTRIBUTE:
    case RPC_ERR_BAD_ATTRIBUTE:
    case RPC_ERR_UNKNOWN_ATTRIBUTE:
	break;
    case RPC_ERR_DATA_MISSING:
	if (interr != ERR_NCX_MISSING_CHOICE) {
	    return err;
	}
	break;
    default:
	if (error_parm && parmtyp==NCX_NT_STRING) {
	    badval = (const xmlChar *)error_parm;
	} else {
	    return err;
	}
    } 

    /* add the required error-info, call even if err2 is NULL */
    res = add_base_vars(err, 
			rpcerr, 
			errnode, 
			badval, 
			badns, 
			badnsid1,
			badnsid2,
			err1, 
			err2, 
			err4);
    if (res != NO_ERR) {
	/*** USE THIS ERROR NODE WITHOUT ALL THE VARS ANYWAY ***/
	;    /* add error statistics (TBD) */
    }

    return err;

} /* agt_rpcerr_gen_error_errinfo */


/********************************************************************
* FUNCTION agt_rpc_gen_insert_error
*
* Generate an internal <rpc-error> record for an element
* for an insert operation failed error
*
* INPUTS:
*   layer == protocol layer where the error occurred
*   interr == internal error code
*             if NO_ERR than use the rpcerr only
*   errval == pointer to the node with the insert error
*  error_path == malloced string of the value (or type, etc.) instance
*                ID string in NCX_IFMT_XPATH format; this will be added
*                to the rpc_err_rec_t and freed later
*             == NULL if not available
*
* RETURNS:
*   pointer to allocated and filled in rpc_err_rec_t struct
*     ready to add to the msg->rpc_errQ
*   NULL if a record could not be allocated or not enough
*     val;id info in the parameters 
*********************************************************************/
rpc_err_rec_t *
    agt_rpcerr_gen_insert_error (ncx_layer_t layer,
				 status_t   interr,
				 const val_value_t *errval,
				 xmlChar *error_path)
{
    rpc_err_rec_t            *err;
    xmlChar                  *error_msg;
    const xmlChar            *badval, *msg, *apptag;
    const void               *err2, *err4;
    xmlns_id_t                badnsid1, badnsid2;
    rpc_err_t                 rpcerr;
    status_t                  res;
    rpc_err_sev_t             errsev;

#ifdef DEBUG
    if (!errval) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    badval = NULL;
    err2 = NULL;
    err4 = NULL;
    badnsid1 = 0;
    badnsid2 = 0;

    rpcerr = RPC_ERR_BAD_ATTRIBUTE;
    errsev = RPC_ERR_SEV_ERROR;

    msg = (const xmlChar *)get_error_string(interr);
    error_msg = (msg) ? xml_strdup(msg) : NULL;

    apptag = (const xmlChar *)"missing-instance"; /* 13.5 */

    /* get a new error record */
    err = start_err(layer, 
		    interr, 
		    rpcerr, 
		    errsev, 
		    apptag, 
		    error_path, 
		    error_msg, 
		    NULL);
    if (!err) {
	if (error_msg) {
	    m__free(error_msg);
	}
	return NULL;
    }

    if (errval->editvars) {
	badval = errval->editvars->insertstr;
    }

    badnsid1 = xmlns_yang_id();
    if (errval->obj->objtype == OBJ_TYP_LIST) {
	err2 = (const void *)NCX_EL_KEY;
    } else {
	err2 = (const void *)NCX_EL_VALUE;
    }
    badnsid2 = val_get_nsid(errval);
    err4 = (const void *)errval->name;

    /* add the required error-info, call even if err2 is NULL */
    res = add_base_vars(err, 
			rpcerr, 
			NULL, 
			badval, 
			NULL, 
			badnsid1,
			badnsid2,
			NULL, 
			err2, 
			err4);
    if (res != NO_ERR) {
	/*** USE THIS ERROR NODE WITHOUT ALL THE VARS ANYWAY ***/
	;    /* add error statistics (TBD) */
    }

    return err;

} /* agt_rpcerr_gen_insert_error */


/********************************************************************
* FUNCTION agt_rpc_gen_unique_error
*
* Generate an internal <rpc-error> record for an element
* for a unique-stmt failed error (data-not-unique)
*
* INPUTS:
*   msghdr == message header to use for prefix storage
*   layer == protocol layer where the error occurred
*   interr == internal error code
*             if NO_ERR than use the rpcerr only
*   errval == pointer to the node with the insert error
*   valuniqueQ == Q of val_unique_t structs to
*                   use for <non-unique> elements
*  error_path == malloced string of the value (or type, etc.) instance
*                ID string in NCX_IFMT_XPATH format; this will be added
*                to the rpc_err_rec_t and freed later
*             == NULL if not available
*
* RETURNS:
*   pointer to allocated and filled in rpc_err_rec_t struct
*     ready to add to the msg->rpc_errQ
*   NULL if a record could not be allocated or not enough
*     val;id info in the parameters 
*********************************************************************/
rpc_err_rec_t *
    agt_rpcerr_gen_unique_error (xml_msg_hdr_t *msghdr,
				 ncx_layer_t layer,
				 status_t   interr,
				 const dlq_hdr_t *valuniqueQ,
				 xmlChar *error_path)
{
    rpc_err_rec_t            *err;
    rpc_err_info_t           *errinfo;
    xmlChar                  *error_msg, *pathbuff;
    const xmlChar            *msg, *apptag;
    val_unique_t             *unival;
    rpc_err_t                 rpcerr;
    status_t                  res;
    rpc_err_sev_t             errsev;
    xmlns_id_t                yangid;


    rpcerr = RPC_ERR_OPERATION_FAILED;
    errsev = RPC_ERR_SEV_ERROR;
    msg = (const xmlChar *)get_error_string(interr);
    error_msg = (msg) ? xml_strdup(msg) : NULL;
    apptag = (const xmlChar *)"data-not-unique"; /* 13.1 */

    /* get a new error record */
    err = start_err(layer, 
		    interr, 
		    rpcerr, 
		    errsev, 
		    apptag, 
		    error_path, 
		    error_msg, 
		    NULL);
    if (!err) {
	if (error_msg) {
	    m__free(error_msg);
	}
	return NULL;
    }

    if (!valuniqueQ) {
	return err;
    }

    yangid = xmlns_yang_id();

    for (unival = (val_unique_t *)dlq_firstEntry(valuniqueQ);
	 unival != NULL;
	 unival = (val_unique_t *)dlq_nextEntry(unival)) {

	pathbuff = NULL;
	res = val_gen_instance_id(msghdr, 
				  unival->valptr, 
				  NCX_IFMT_XPATH1, 
				  &pathbuff);
	if (res != NO_ERR) {
	    return err;
	}

	errinfo = rpc_err_new_info();
	if (!errinfo) {
	    /* try to send the error as-is */
	    m__free(pathbuff);
	    return err;
	}
	
	/* generate non-unique value */
	errinfo->name_nsid = yangid;
	errinfo->name = NCX_EL_NON_UNIQUE;
	errinfo->val_btype = NCX_BT_INSTANCE_ID;
	errinfo->val_nsid = 0;
	errinfo->dval = pathbuff;
	errinfo->v.strval = errinfo->dval;
	dlq_enque(errinfo, &err->error_info);
    }

    return err;

} /* agt_rpcerr_gen_unique_error */


/********************************************************************
* FUNCTION agt_rpc_gen_attr_error
*
* Generate an internal <rpc-error> record for an attribute
*
* INPUTS:
*   layer == protocol layer where the error occurred
*   interr == internal error code
*             if NO_ERR than use the rpcerr only
*   attr   == attribute that had the error
*   errnode == XML node where error occurred
*           == NULL then there is no valid XML node (maybe the error!)
*   badns == URI string of the namespace that is bad (or NULL)
*
* RETURNS:
*   pointer to allocated and filled in rpc_err_rec_t struct
*     ready to add to the msg->rpc_errQ (or add more error-info)
*   NULL if a record could not be allocated or not enough
*     valid info in the parameters to generate an error report
*********************************************************************/
rpc_err_rec_t *
    agt_rpcerr_gen_attr_error (ncx_layer_t layer,
			       status_t   interr,
			       const xml_attr_t *attr,
			       const xml_node_t *errnode,
			       const xmlChar *badns,
			       xmlChar *error_path)
{
    rpc_err_rec_t  *err;
    rpc_err_sev_t   errsev;
    rpc_err_t       rpcerr;
    xmlChar        *badval, *error_msg;
    const void     *err1, *err2;
    const xmlChar  *msg;
    status_t        res;
    xmlns_id_t      badnsid1, badnsid2;

    badval = NULL;
    badnsid1 = 0;
    badnsid2 = 0;
    err1 = NULL;

    if (attr) {
	badnsid1 = attr->attr_ns;
	err2 = (const void *)attr->attr_name;
    } else {
	err2 = NULL;
    }

    rpcerr = get_rpcerr(interr, FALSE, &errsev); 

    /* check the rpcerr out the required error-info */
    switch (rpcerr) {
    case RPC_ERR_UNKNOWN_ATTRIBUTE:
    case RPC_ERR_MISSING_ATTRIBUTE:
	break;
    case RPC_ERR_INVALID_VALUE:
	rpcerr = RPC_ERR_BAD_ATTRIBUTE;
	/* fall through */
    case RPC_ERR_BAD_ATTRIBUTE:
	if (attr) {
	    badval = attr->attr_val;
	}
	break;
    case RPC_ERR_UNKNOWN_NAMESPACE:
	break;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
	return NULL;
    } 

    /* generate a default error message */
    msg = (const xmlChar *)get_error_string(interr);
    error_msg = (msg) ? xml_strdup(msg) : NULL;

    /* get a new error record */
    err = start_err(layer, 
		    interr, 
		    rpcerr, 
		    errsev, 
		    NULL, 
		    error_path, 
		    error_msg, 
		    NULL);
    if (!err) {
	if (error_msg) {
	    m__free(error_msg);
	}
	return NULL;
    }

    /* add the required error-info */
    res = add_base_vars(err, 
			rpcerr, 
			errnode, 
			badval, 
			badns,
			badnsid1,
			badnsid2,
			err1, 
			err2, 
			NULL);
    if (res != NO_ERR) {
	/*** USE THIS ERROR NODE WITHOUT ALL THE VARS ANYWAY ***/
	;    /* add error statistics (TBD) */
    }

    return err;

} /* agt_rpcerr_gen_attr_error */


/* END file agt_rpcerr.c */
