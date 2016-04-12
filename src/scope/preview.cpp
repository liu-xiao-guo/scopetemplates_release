#include <scope/preview.h>

#include <unity/scopes/ColumnLayout.h>
#include <unity/scopes/PreviewWidget.h>
#include <unity/scopes/PreviewReply.h>
#include <unity/scopes/Result.h>
#include <unity/scopes/VariantBuilder.h>

#include <iostream>
#include <sstream>
#include <QDebug>

namespace sc = unity::scopes;
using namespace sc;
using namespace std;
using namespace scope;

Preview::Preview(const sc::Result &result, const sc::ActionMetadata &metadata) :
    sc::PreviewQueryBase(result, metadata) {
}

void Preview::cancelled() {
}

void Preview::run(sc::PreviewReplyProxy const& reply) {
    Result result = PreviewQueryBase::result();

    ColumnLayout layout1col(1);
    std::vector<std::string> ids = { "image", "header", "actionsId", "summary", "tracks",
                                    "videos", "gallery_header", "gallerys", "reviews", "exp",
                                     "review_input", "rating_input", "inputId", "img" };
//    std::vector<std::string> ids = { "inputId", "img" };

    PreviewWidgetList widgets;

    ColumnLayout layout2col(2);
    layout2col.add_column(ids);
    layout2col.add_column({});

    ColumnLayout layout3col(3);
    layout3col.add_column(ids);
    layout3col.add_column({});
    layout3col.add_column({});

    QString urlString(result["uri"].get_string().c_str());

    // Create an Open button and provide the URI to open for this preview result
    PreviewWidget w_actions("actionsId", "actions");
    VariantBuilder builder1;
    builder1.add_tuple({
            {"id", Variant("open")},
            {"label", Variant("Open")},
            {"uri", Variant(urlString.toStdString())} // uri set, this action will be handled by the Dash
        });
    w_actions.add_attribute_value("actions", builder1.end());
    widgets.emplace_back(w_actions);

    // Define the header section
    sc::PreviewWidget header("header", "header");
    // It has title and a subtitle properties
    header.add_attribute_mapping("title", "title");
    header.add_attribute_mapping("subtitle", "subtitle");
    widgets.emplace_back(header);

    // Define the image section
    sc::PreviewWidget image("image", "image");
    // It has a single source property, mapped to the result's art property
    image.add_attribute_mapping("source", "art");
    widgets.emplace_back(image);

    // Define the summary section
    sc::PreviewWidget description("summary", "text");
    // It has a text property, mapped to the result's description property
    description.add_attribute_mapping("text", "description");
    widgets.emplace_back(description);

    PreviewWidget listen("tracks", "audio");
    {
        VariantBuilder builder;
        builder.add_tuple({
            {"title", Variant("This is the song title")},
            {"source", Variant(result["musicSource"].get_string().c_str())}
        });
        listen.add_attribute_value("tracks", builder.end());
    }

    PreviewWidget video("videos", "video");
    video.add_attribute_value("source", Variant(result["videoSource"].get_string().c_str()));
    video.add_attribute_value("screenshot", Variant(result["screenshot"].get_string().c_str()));

    PreviewWidget header_gal("gallery_header", "header");
    header_gal.add_attribute_value("title", Variant("Gallery files are:"));

    PreviewWidget gallery("gallerys", "gallery");
    gallery.add_attribute_mapping("sources", "array");

    if ( result["musicSource"].get_string().length() != 0 ) {
        widgets.emplace_back(listen);
    }

    if( result["videoSource"].get_string().length() != 0 ) {
        widgets.emplace_back(video);
    }

    if( result["array"].get_array().size() != 0 ) {
        widgets.emplace_back(header_gal);
        widgets.emplace_back(gallery);
    }

    // The following shows the review
    PreviewWidget review("reviews", "reviews");
    VariantBuilder builder;
    builder.add_tuple({
                          {"author", Variant("John Doe")},
                          {"review", Variant("very good")},
                          {"rating", Variant(3.5)}
                      });
    builder.add_tuple({
                          {"author", Variant("Mr. Smith")},
                          {"review", Variant("very poor")},
                          {"rating", Variant(5)}
                      });
    review.add_attribute_value("reviews", builder.end());
    widgets.emplace_back(review);

    // The following shows the expandable
    PreviewWidget expandable("exp", "expandable");
    expandable.add_attribute_value("title", Variant("This is an expandable widget"));
    expandable.add_attribute_value("collapsed-widgets", Variant(1));
    PreviewWidget w1("w1", "text");
    w1.add_attribute_value("title", Variant("Subwidget 1"));
    w1.add_attribute_value("text", Variant("A text"));
    PreviewWidget w2("w2", "text");
    w2.add_attribute_value("title", Variant("Subwidget 2"));
    w2.add_attribute_value("text", Variant("A text"));
    expandable.add_widget(w1);
    expandable.add_widget(w2);
    widgets.emplace_back(expandable);

    // The following shows a review rating-input
    PreviewWidget w_review("review_input", "rating-input");
    w_review.add_attribute_value("submit-label", Variant("Send"));
    w_review.add_attribute_value("visible", Variant("review"));
    w_review.add_attribute_value("required", Variant("review"));
    std::string reply_label = "Reply";
    std::string max_chars_label = "140 characters max";
    w_review.add_attribute_value("review-label", Variant(reply_label + ": " + max_chars_label));
    widgets.emplace_back(w_review);

    // The follwing shows a rating rating-input
    PreviewWidget w_rating("rating_input", "rating-input");
    w_rating.add_attribute_value("visible", Variant("rating"));
    w_rating.add_attribute_value("required", Variant("rating"));
    w_rating.add_attribute_value("rating-label", Variant("Please rate this"));
    widgets.emplace_back(w_rating);

    PreviewWidget w_image("img", "image");
    w_image.add_attribute_value("source", Variant(result["share_icon"].get_string().c_str()));
    VariantMap share_data;
//    share_data["uri"] = Variant("http://img2.imgtn.bdimg.com/it/u=442803940,143587648&fm=21&gp=0.jpg");
    share_data["uri"] = Variant(result["share_pic"].get_string().c_str());
    share_data["content-type"] = Variant("pictures");
    w_image.add_attribute_value("share-data",  sc::Variant(share_data));
    widgets.emplace_back(w_image);

    PreviewWidget w_commentInput("inputId", "comment-input");
    w_commentInput.add_attribute_value("submit-label", Variant("Post"));
    widgets.emplace_back(w_commentInput);

    // In the following, fake some comments data
    QList<Comment> comment_list;
    std::string comment_str = "Comment ";
    for(int i = 0; i < 3;  i++) {
        Comment comment;

        comment.id         = 1.0;
        comment.publishTime = "2015-3-18";
        comment.text = comment_str;
        comment.text += std::to_string(i+1);
        comment_list.append(comment);
    }

    int index = 0;
    Q_FOREACH(const auto & comment, comment_list) {
        std::string id = "commentId_" + std::to_string(index++);
        ids.emplace_back(id);

        PreviewWidget w_comment(id, "comment");
        w_comment.add_attribute_value("comment", Variant(comment.text));
        w_comment.add_attribute_value("author", Variant("Author"));
        w_comment.add_attribute_value("source", Variant(result["comment_icon"].get_string().c_str()));
        w_comment.add_attribute_value("subtitle", Variant(comment.publishTime));
        widgets.emplace_back(w_comment);
    }

    layout1col.add_column(ids);
    reply->register_layout({layout1col, layout2col, layout3col});
    reply->push( widgets );
}

