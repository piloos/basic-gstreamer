#ifndef PIPELINE_H
#define PIPELINE_H

#include <QObject>

#include <gst/gst.h>

class Pipeline : public QObject
{
    Q_OBJECT
public:
    explicit Pipeline(QObject *parent = 0);
    
    GstElement *pipeline;
    GstElement *videotestsrc;
    GstElement *ximagesink;
    
signals:
    
public slots:
    
};

#endif // PIPELINE_H
