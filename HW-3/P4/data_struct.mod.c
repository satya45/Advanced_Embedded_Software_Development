#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa88a79f7, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x77ce4795, __VMLINUX_SYMBOL_STR(param_ops_long) },
	{ 0xafce57c4, __VMLINUX_SYMBOL_STR(param_array_ops) },
	{ 0xd23d2495, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x4f52b1bf, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x11f74a9a, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x310917fe, __VMLINUX_SYMBOL_STR(sort) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x4f68e5c9, __VMLINUX_SYMBOL_STR(do_gettimeofday) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D162D92C61E9CE43C8BE819");
