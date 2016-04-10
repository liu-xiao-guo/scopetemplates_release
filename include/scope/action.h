#ifndef SCOPE_ACTION_H_
#define SCOPE_ACTION_H_

#include <unity/scopes/ActionMetadata.h>
#include <unity/scopes/ActivationQueryBase.h>
#include <unity/scopes/ActivationResponse.h>
#include <unity/scopes/Result.h>

class Action : public unity::scopes::ActivationQueryBase
{
public:
    Action(unity::scopes::Result const& result,
           unity::scopes::ActionMetadata const& metadata,
           std::string const& widget_id,
           std::string const& action_id);
    ~Action() = default;

     virtual unity::scopes::ActivationResponse activate() override;
};

#endif // SCOPE_ACTION_H_

