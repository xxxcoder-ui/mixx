#pragma once

#include <QRegularExpression>
#include <QString>
#include <QtSql>

#include "library/searchquery.h"
#include "library/trackcollection.h"
#include "util/class.h"

class SearchQueryParser {
  public:
    explicit SearchQueryParser(TrackCollection* pTrackCollection);

    virtual ~SearchQueryParser();

    std::unique_ptr<QueryNode> parseQuery(
            const QString& query,
            const QStringList& searchColumns,
            const QString& extraFilter) const;


  private:
    void parseTokens(QStringList tokens,
                     QStringList searchColumns,
                     AndNode* pQuery) const;

    QString getTextArgument(QString argument,
                            QStringList* tokens) const;

    TrackCollection* m_pTrackCollection;
    QStringList m_textFilters;
    QStringList m_numericFilters;
    QStringList m_specialFilters;
    QStringList m_ignoredColumns;
    QStringList m_allFilters;
    QHash<QString, QStringList> m_fieldToSqlColumns;

    QRegularExpression m_fuzzyMatcher;
    QRegularExpression m_textFilterMatcher;
    QRegularExpression m_crateFilterMatcher;
    QRegularExpression m_numericFilterMatcher;
    QRegularExpression m_specialFilterMatcher;

    DISALLOW_COPY_AND_ASSIGN(SearchQueryParser);
};
