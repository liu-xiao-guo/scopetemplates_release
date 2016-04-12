#ifndef SCOPE_QUERY_H_
#define SCOPE_QUERY_H_

#include <api/client.h>

#include <unity/scopes/SearchQueryBase.h>
#include <unity/scopes/ReplyProxyFwd.h>
#include <string>

#include <QList>

using namespace std;

namespace scope {

/**
 * Represents an individual query.
 *
 * A new Query object will be constructed for each query. It is
 * given query information, metadata about the search, and
 * some scope-specific configuration.
 */
class Query: public unity::scopes::SearchQueryBase {
public:
    Query(const unity::scopes::CannedQuery &query,
          const unity::scopes::SearchMetadata &metadata,
          QString scopePath, api::Config::Ptr config);

    ~Query() = default;
    void cancelled() override;
    void run(const unity::scopes::SearchReplyProxy &reply) override;

private:
    void pushResult(unity::scopes::SearchReplyProxy const& reply,
                    const string renderer, int i);
    void pushResult(const unity::scopes::SearchReplyProxy &reply,
                    const std::shared_ptr<const unity::scopes::Category> *cat, int i,
                    bool linkquery = false);

private:
    QString scopePath_;
    api::Client client_;
    QList<QString> images_;
    QList<QString> icons_;
    QString background_;
};

}

#endif // SCOPE_QUERY_H_


