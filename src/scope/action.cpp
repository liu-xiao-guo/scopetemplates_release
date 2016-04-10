#include <scope/action.h>
#include <unity/scopes/ActivationResponse.h>
#include <unity/UnityExceptions.h>

#include <QString>
#include <QDebug>

#include <iostream>

namespace sc = unity::scopes;
using namespace std;

QString qstr_(std::string str)
{
    return QString::fromStdString(str);
}

Action::Action(const unity::scopes::Result &result,
               const unity::scopes::ActionMetadata &metadata,
               std::string const& widget_id,
               std::string const& action_id)
    : sc::ActivationQueryBase(result, metadata, widget_id, action_id)
{
}

sc::ActivationResponse Action::activate()
{
    qDebug() << "action id in activate: " << qstr_(action_id());

    QString review = QString("%1").arg(qstr_(action_metadata().scope_data().
                                              get_dict()["review"].get_string()));

    double rating = action_metadata().scope_data().
                          get_dict()["rating"].get_double();

    qDebug() << "review: " << review;
    qDebug() << "rating: " << rating;

    sc::ActivationResponse done(sc::ActivationResponse::ShowDash);
    cerr << "activate called" << endl;
    return done;
}
