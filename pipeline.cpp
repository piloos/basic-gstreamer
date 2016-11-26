#include "pipeline.h"
#include <QtDebug>
#include <QTime>
#include <QCoreApplication>

void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

static void cb_message (GstBus *bus, GstMessage *msg, gpointer user_data) {
    Q_UNUSED(bus);

    Pipeline *pipeline = (Pipeline*) user_data;

    QString timestamp = QTime::currentTime().toString("hh:mm:ss.zzz");

    switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_ERROR:
    {
        GError *err;
        gchar *debug;

        gst_message_parse_error (msg, &err, &debug);
        qCritical() << QString("[Bus] %1").arg(timestamp) << QString("Error on element %2: %1").arg(GST_MESSAGE_SRC_NAME(msg)).arg(err->message);
        g_error_free (err);
        g_free (debug);

        gst_element_set_state (pipeline->pipeline, GST_STATE_READY);
        break;
    }
    case GST_MESSAGE_EOS:
    {
        /* end-of-stream */
        qDebug() << QString("[Bus] %1").arg(timestamp) << QString("EOS reached");
        gst_element_set_state (pipeline->pipeline, GST_STATE_READY);
        break;
    }
    case GST_MESSAGE_STATE_CHANGED:
    {
        GstState old_state, new_state;
        gst_message_parse_state_changed (msg, &old_state, &new_state, NULL);
        qDebug() << QString("[Bus] %1").arg(timestamp) << QString("Element %1 changed state from %2 to %3")
                    .arg(GST_MESSAGE_SRC_NAME(msg))
                    .arg(gst_element_state_get_name (old_state))
                    .arg(gst_element_state_get_name (new_state));
        break;
    }
    case GST_MESSAGE_ELEMENT:
    {
        const GstStructure *msg_struct = gst_message_get_structure(msg);
        gchar *s = gst_structure_to_string (msg_struct);
        qDebug() << QString("[Bus] %1").arg(timestamp) << QString("Element specific message from element %1: %2").arg(GST_MESSAGE_SRC_NAME(msg)).arg(s);
        g_free(s);
        break;
    }
    case GST_MESSAGE_STREAM_STATUS:
    {
        GstStreamStatusType stream_status_type;
        GstElement *owner;
        gst_message_parse_stream_status(msg, &stream_status_type, &owner);
        qDebug() << QString("[Bus] %1").arg(timestamp) << QString("Stream status change on element %1: %2")
                    .arg(GST_OBJECT_NAME(owner))
                    .arg(stream_status_type);
        break;
    }
    default:
    {
        /* Unhandled message */
        qDebug() << QString("[Bus] %1").arg(timestamp) << QString("Unhandled: %1 message from element %2").arg(GST_MESSAGE_TYPE_NAME(msg)).arg(GST_MESSAGE_SRC_NAME(msg));
        break;
    }
    }
}

Pipeline::Pipeline(QObject *parent) :
    QObject(parent)
{
    pipeline = gst_pipeline_new(NULL);

    videotestsrc = gst_element_factory_make("videotestsrc", NULL);
    ximagesink = gst_element_factory_make("ximagesink", NULL);

    if(!pipeline || !videotestsrc || !ximagesink) {
        qCritical() << QString("Could not create all elements!");
    }

    GstBus *bus = gst_element_get_bus(pipeline);
    gst_bus_add_signal_watch(bus);
    g_signal_connect(bus, "message", G_CALLBACK (cb_message), this);
    gst_object_unref(bus);

    /* Start playing */
    GstStateChangeReturn ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        qCritical() << QString("Unable to set the pipeline to the playing state!");
        gst_object_unref (pipeline);
    }
    else {
        qDebug() << QString("Pipeline state change return: %1").arg(ret);
    }

    gst_bin_add(GST_BIN(pipeline), videotestsrc);
    gst_bin_add(GST_BIN(pipeline), ximagesink);

    gst_element_link(videotestsrc, ximagesink);

    delay(1000);
    //gst_element_sync_state_with_parent(videotestsrc);
    gst_element_set_state(videotestsrc, GST_STATE_READY);
    delay(1000);
    gst_element_sync_state_with_parent(ximagesink);
    delay(1000);
    gst_element_set_state(videotestsrc, GST_STATE_PLAYING);
    /*ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        qCritical() << QString("Unable to set the pipeline to the playing state!");
        gst_object_unref (pipeline);
    }
    else {
        qDebug() << QString("Pipeline state change return: %1").arg(ret);
    }*/
}
