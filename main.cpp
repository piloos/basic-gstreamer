#include <QCoreApplication>
#include <gst/gst.h>
#include <pipeline.h>

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);
    
    QCoreApplication a(argc, argv);
    
    Pipeline p;
    
    return a.exec();
    
    gst_deinit();
}
