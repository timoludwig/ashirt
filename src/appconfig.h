// New AppConfig 2022, Chris Rizzitello
// Licensed under the terms of MIT. See LICENSE file in project root for terms.

#pragma once

#include <QObject>
#include <QStandardPaths>
#include <QSettings>
#include <QVariant>
#include <QRegularExpression>

#include "models/tag.h"

struct CONFIG {
    inline static const auto EVIDENCEREPO = QStringLiteral("evidenceRepo");
    inline static const auto ACCESSKEY = QStringLiteral("accessKey");
    inline static const auto SECRETKEY = QStringLiteral("secretKey");
    inline static const auto APIURL = QStringLiteral("apiURL");
    inline static const auto COMMAND_SCREENSHOT = QStringLiteral("screenshotCommand");
    inline static const auto SHORTCUT_SCREENSHOT = QStringLiteral("screenshotShortcut");
    inline static const auto COMMAND_CAPTUREWINDOW = QStringLiteral("captureWindowExec");
    inline static const auto SHORTCUT_CAPTUREWINDOW = QStringLiteral("captureWindowShortcut");
    inline static const auto SHORTCUT_CAPTURECLIPBOARD = QStringLiteral("captureClipboardShortcut");
};

/// AppConfig is a singleton for accessing the application's configuration.
class AppConfig : public QObject
{
    Q_OBJECT
public:
    /// Access the AppConfig for connections
    static AppConfig* get() {
        static AppConfig instance;
        return &instance;
    }
    /// Request the value of a key; returns a QString version of that value or empty QString
    static QString value(const QString &key = QString());
    /// Sets a value of a key to any QString Only accepts QStrings
    static void setValue(const QString &key = QString(), const QString &value = QString());
    /// Returns a List of all the Valid Keys in the config file
    static QStringList appConfigKeys() { return _appConfigValidKeys; }
    /// Writes a Copy of the config to FileName. Returns true if successful
    static bool exportConfig(const QString &fileName = QString());
    /// Import configuration from a file.
    static void importConfig(const QString &fileName = QString());
    /// Helper to get the operation Name
    static QString operationName();
    /// Helper to get the operation Slug
    static QString operationSlug();
    /// Helper to write operation Details, emits operationsChanged when called.
    static void setOperationDetails(const QString &operationSlug = QString(), const QString &operationName = QString());
    /// Get the list of the last used tag(s)
    static QList<model::Tag> getLastUsedTags();
    /// Set the last used Tags
    static void setLastUsedTags(QList<model::Tag> lastTags);

signals:
    void operationChanged(QString operationSlug, QString operationName);

private:
    AppConfig(QObject *parent = nullptr);
    ~AppConfig() = default;
    AppConfig(AppConfig const &) = delete;
    void operator=(AppConfig const &) = delete;
    void validateConfig();
    // Helpers for JSON Format
    static bool readJSONSettings(QIODevice &device, QMap<QString, QVariant> &map);
    static bool writeJSONSettings(QIODevice &device, const QMap<QString, QVariant> &map);
    inline static const auto JSON = QSettings::registerFormat(QStringLiteral("json"), &readJSONSettings, &writeJSONSettings);
    //Vars Used Internally
    QSettings *appConfig = nullptr;
    QSettings *appSettings = nullptr;
#ifdef Q_OS_MAC
    inline static const auto _configFile = QStringLiteral("%1/ashirt/config.json").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
#else
    inline static const QString _configFile = QStringLiteral("%1/ashirt/config.json").arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
#endif
    inline static const auto _opSlugSetting = QStringLiteral("operation/slug");
    inline static const auto _opNameSetting = QStringLiteral("operation/name");
    inline static const auto _lastUsedTagsSetting = QStringLiteral("gather/tags");
    inline static const QStringList _appConfigValidKeys {
        CONFIG::EVIDENCEREPO,
        CONFIG::ACCESSKEY,
        CONFIG::SECRETKEY,
        CONFIG::APIURL,
        CONFIG::COMMAND_SCREENSHOT,
        CONFIG::SHORTCUT_SCREENSHOT,
        CONFIG::COMMAND_CAPTUREWINDOW,
        CONFIG::SHORTCUT_CAPTUREWINDOW,
        CONFIG::SHORTCUT_CAPTURECLIPBOARD,
    };
};
