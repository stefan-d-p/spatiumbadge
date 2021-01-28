#include <spatiumbadge.hpp>

ACTION spatiumbadge::signup(const name& issuer, const std::string& org_name,
                            const std::string& url, const std::string& cid_image) {

   require_auth(issuer);
   check(org_name.length() > 0,"You must provider an organization name");
   check(url.length() > 0, "You must provide an url");
}

ACTION spatiumbadge::createclass(const name& issuer, const name& badgeclass_name, const std::string& name,
                                 const std::string& description, const std::string& cid_image,
                                 const std::string& cid_meta) {

   require_auth(issuer);
   check(name.length() > 0, "you must provide a name for the new class");
   check(name.length() < 256, "name must not exceed 256 characters");
   check(description.length() > 0,"you must provide a description");
   check(description.length() < 512,"description must not exceed 512 characters");

   badgeclasses_t badgeclasses(get_self(), issuer.value);
   auto bcitr = badgeclasses.find(badgeclass_name.value);
   check(bcitr == badgeclasses.end(),"a badgeclass with that name already exists");

   bcitr = badgeclasses.emplace(issuer, [&](auto& _badgeclass) {
      _badgeclass.badgeclass_name = badgeclass_name;
      _badgeclass.name = name;
      _badgeclass.description = description;
      _badgeclass.cid_badge_image = cid_image;
      _badgeclass.cid_badge_meta = cid_meta;
   });

   require_recipient(issuer);

}

ACTION spatiumbadge::issue(const name& issuer, const name& owner, 
                           const name& badgeclass, const std::string& memo) {

   require_auth(issuer);
   check(is_account(owner),"The owner is not a valid account");
   check(memo.length() < 256, "descriptive memo must be less thank 256 characters");

   badgeclasses_t badgeclasses = badgeclasses_t(get_self(),issuer.value);
   
   auto bcitr = badgeclasses.require_find(badgeclass.value,"Badgeclass does not exist");

   uint64_t next_proposal_id = proposals.available_primary_key();

   auto propitr = proposals.emplace(issuer, [&](auto& _proposal) {
      _proposal.proposal_id = next_proposal_id;
      _proposal.issuer = issuer;
      _proposal.owner = owner;
      _proposal.badgeclass = badgeclass;
   });

   receipt_propose_s receipt(*propitr,*bcitr);

   action(
      permission_level{get_self(),"active"_n},
      get_self(),
      "receiptissue"_n,
      receipt
   ).send();
  

}

ACTION spatiumbadge::accept(uint64_t& proposal_id) {
   auto prop_itr = proposals.require_find(proposal_id,"badge proposal does not exist");
   require_auth(prop_itr->owner);

   badges_t badges = badges_t(get_self(),prop_itr->owner.value);

   uint64_t next_badge_id = badges.available_primary_key();

   badges.emplace(prop_itr->owner, [&](auto& _badge) {
      _badge.badge_id = next_badge_id;
      _badge.issuer = prop_itr->issuer;
      _badge.badgeclass = prop_itr->badgeclass;
   });

   proposals.erase(prop_itr);

   // TODO: Send receipt to issuer and owner

}

ACTION spatiumbadge::receiptissue(receipt_propose_s& receipt) {
   require_auth(get_self());
   require_recipient(receipt.issuer);
   require_recipient(receipt.issuer);
}

