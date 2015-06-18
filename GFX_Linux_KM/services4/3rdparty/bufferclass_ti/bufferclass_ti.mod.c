#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x5af47c2, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x1c39ccce, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xff178f6, __VMLINUX_SYMBOL_STR(__aeabi_idivmod) },
	{ 0xfbc74f64, __VMLINUX_SYMBOL_STR(__copy_from_user) },
	{ 0x98af12d, __VMLINUX_SYMBOL_STR(arm_dma_ops) },
	{ 0x9a1c5d28, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x8361e18f, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0xaaccb859, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x328a05f1, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0x4103d69c, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x8ef75419, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xcd89f3d1, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xb3d66bc8, __VMLINUX_SYMBOL_STR(PVRGetBufferClassJTable) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x2a9b0b2f, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xfca84d55, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x6548b408, __VMLINUX_SYMBOL_STR(__class_create) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=pvrsrvkm";


MODULE_INFO(srcversion, "2C6C01BEBC88D7B8CDDBD43");
