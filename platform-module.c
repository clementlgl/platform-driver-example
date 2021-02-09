#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

static struct platform_device *pdev;

static int data_show(struct device *dev, struct device_attribute *attr, char *buf){
    return sprintf(buf, "Hello\n");
}

static DEVICE_ATTR(data, 0444, data_show, NULL);

static int platform_module_probe (struct platform_device *pdev){
    pr_info("Driver probed!\n");

    int err = device_create_file(&pdev->dev, &dev_attr_data);
   
    return 0;
}

static int platform_module_remove(struct platform_device *pdev){
    pr_info("Driver remove!\n");
    device_remove_file(&pdev->dev, &dev_attr_data);

    return 0;
}

static struct platform_driver platform_module_driver={
    .probe = platform_module_probe,
    .remove = platform_module_remove,
    .driver = {
        .name="platform-module",
    },
};

static int __init platform_module_init(void)
{
    platform_driver_register(&platform_module_driver);
    pdev = platform_device_register_simple("platform-module", 0, NULL, 0);
    if(IS_ERR(pdev))
    {
        platform_driver_unregister(&platform_module_driver);
        return PTR_ERR(pdev);
    }
    return 0;
}

static void __exit platform_module_exit(void)
{
    platform_device_unregister(pdev);
    platform_driver_unregister(&platform_module_driver);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Clément Laigle");
MODULE_DESCRIPTION("Simple platform driver with attribute");

module_init(platform_module_init);
module_exit(platform_module_exit);