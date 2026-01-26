/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 1102939ccafb1f8a6ffd9e2868755b05a8992e05 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Iandenh_Unleash_Client_getOrCreate, 0, 3, Iandenh\\\125nleash\\Client, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, upstream, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, token, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, disableMetrics, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, streamingMode, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, debugMode, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, refreshIntervalMilliseconds, IS_LONG, 0, "15000")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, metricsIntervalMilliseconds, IS_LONG, 0, "60000")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Iandenh_Unleash_Client_isEnabled, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, featureFlag, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, context, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Iandenh_Unleash_Client_close, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Iandenh_Unleash_Client, getOrCreate);
ZEND_METHOD(Iandenh_Unleash_Client, isEnabled);
ZEND_METHOD(Iandenh_Unleash_Client, close);

static const zend_function_entry class_Iandenh_Unleash_Client_methods[] = {
	ZEND_ME(Iandenh_Unleash_Client, getOrCreate, arginfo_class_Iandenh_Unleash_Client_getOrCreate, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Iandenh_Unleash_Client, isEnabled, arginfo_class_Iandenh_Unleash_Client_isEnabled, ZEND_ACC_PUBLIC)
	ZEND_ME(Iandenh_Unleash_Client, close, arginfo_class_Iandenh_Unleash_Client_close, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_Iandenh_Unleash_Client(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Iandenh\\Unleash", "Client", class_Iandenh_Unleash_Client_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
