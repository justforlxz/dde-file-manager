/*
 * Copyright (C) 2019 Deepin Technology Co., Ltd.
 *
 * Author:     Gary Wang <wzc782970009@gmail.com>
 *
 * Maintainer: Gary Wang <wangzichong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <QObject>
#include <QWidget>
#include <QMenu>

#include "durl.h"
#include "dfmglobal.h"
#include "interfaces/dabstractfileinfo.h"
#include <ddiskmanager.h>

DFM_BEGIN_NAMESPACE

class DFMSideBarView;
class DFMSideBarModel;
class DFMSideBarItem;
class DFMSideBar : public QWidget
{
    Q_OBJECT
public:
    enum GroupName {
        Common,
        Device,
        Bookmark,
        Network,
        Tag,
        Other,
        Unknow
    };

    explicit DFMSideBar(QWidget *parent = nullptr);
    QWidget *sidebarView(); // return m_sidebarView

    QRect groupGeometry(const QString &groupName);
    void scrollToGroup(const QString &groupName);

    void setCurrentUrl(const DUrl &url);

    int addItem(DFMSideBarItem *item, const QString &group);
    bool removeItem(const DUrl &url, const QString &group);
    int findItem(const DFMSideBarItem * item) const;
    int findItem(const DUrl &url, const QString &group) const;
    int findItem(const DUrl &url, bool fuzzy = false) const;
    int findItem(std::function<bool(const DFMSideBarItem *item)> cb) const; // cb return true to get the index
    int findLastItem(const QString &group, bool sidebarItemOnly = true) const;
    void openItemEditor(int index) const;
    QSet<QString> disableUrlSchemes() const;

    void setContextMenuEnabled(bool enabled);
    void setDisableUrlSchemes(const QSet<QString> &schemes);

    DUrlList savedItemOrder(const QString &groupName) const;
    void saveItemOrder(const QString &groupName) const;

    static QString groupName(GroupName group);
    static GroupName groupFromName(const QString &name);

    static const int minimumWidth = 120;
    static const int maximumWidth = 200;
    void rootFileChange();

signals:
    void disableUrlSchemesChanged();
public slots:
    void onRootFileChange(const DAbstractFileInfoPointer &fi);
private slots:
    void onItemActivated(const QModelIndex &index);
    void onContextMenuRequested(const QPoint &pos);
    void onRename(const QModelIndex &index, QString newName) const ;


    /**
     * @brief onUnlockVault
     */
    void onUnlockVault();

    /**
     * @brief onCreateVault
     */
    void onCreateVault();

    /**
     * @brief onRecoverVault
     */
    void onRecoverVault();

    /**
     * @brief onRemoveVault 删除保险箱
     */
    void onRemoveVault();
private:
    void initUI();
    void initModelData();
    void initConnection();
    void initUserShareItem();
    void initRecentItem();
    void initBookmarkConnection();
    void initDeviceConnection();
    void initTagsConnection();
    void initVaultConnection();
    void applySidebarColor();
    void updateSeparatorVisibleState();
    void addGroupItems(GroupName groupType);
    void insertItem(int index, DFMSideBarItem * item, const QString &groupName);
    void appendItem(DFMSideBarItem * item, const QString &groupName);
    void appendItemWithOrder(QList<DFMSideBarItem *> &list, const DUrlList &order, const QString &groupName);

    QModelIndex groupModelIndex(const QString &groupName);

    void changeEvent(QEvent *event) override;

    DFMSideBarView *m_sidebarView;
    DFMSideBarModel *m_sidebarModel;
    QScopedPointer<DDiskManager> m_udisks2DiskManager;
    bool m_contextMenuEnabled = true;
    QList<DUrl> devitems;
    QSet<QString> m_disableUrlSchemes;

};

DFM_END_NAMESPACE
