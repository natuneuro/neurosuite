#ifndef KLUSTERRUBBERBAND_H
#define KLUSTERRUBBERBAND_H

#include <QRubberBand>
#include "libneurosuite_export.h"

class NEUROSUITE_EXPORT KlusterRubberBand : public QRubberBand
{
    Q_OBJECT
public:
    explicit KlusterRubberBand(QRubberBand::Shape s, QWidget *w=0);
    ~KlusterRubberBand();

protected:
    void paintEvent( QPaintEvent *p );
};

#endif // KLUSTERRUBBERBAND_H
