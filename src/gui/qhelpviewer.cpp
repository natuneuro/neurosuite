/*
Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
*/

#include "qhelpviewer.h"
#include <QVBoxLayout>
#include <QWebView>
#include <QWebPage>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDesktopServices>

QHelpViewer::QHelpViewer(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("Handbook"));
    QVBoxLayout *lay = new QVBoxLayout;
    mView = new QWebView;
    lay->addWidget(mView);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    lay->addWidget(buttonBox);
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(mView, SIGNAL(linkClicked(QUrl)), SLOT(slotLinkCLicked(QUrl)));
    setLayout(lay);
    mView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
}

QHelpViewer::~QHelpViewer()
{
}

void QHelpViewer::setHtml(const QString& filename, const QString& anchor)
{
    mView->load(QUrl(filename));
}

void QHelpViewer::slotLinkCLicked(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}
