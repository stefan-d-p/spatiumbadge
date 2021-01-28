#include <eosio/eosio.hpp>
#include <string>
#include <vector>

using namespace eosio;

CONTRACT spatiumbadge : public contract {
   public:
      using contract::contract;

      ACTION signup(const name& issuer, const std::string& org_name,
                    const std::string& url, const std::string& cid_image);
      
      ACTION createclass(const name& issuer, const name& badgeclass_name, const std::string& name,
                         const std::string& description,
                         const std::string& cid_image, const std::string& cid_meta);
      
      ACTION issue(const name& issuer, const name& owner, 
                   const name& badgeclass, const std::string& memo);

      ACTION accept(uint64_t& proposal_id);
      
      

   private:
      
      /*
       * Information about a single issuer, scoped to contract
       */
      TABLE profiles_s {
         name                    issuer;
         std::string             org_name;
         std::string             url;
         std::string             cid_profile_image;

         uint64_t primary_key() const { return issuer.value; };

      };
      
      typedef multi_index<"profiles"_n, profiles_s>            profiles_t;

      /*
       * Information about a badge class, scoped to issuer
       */
      TABLE badgeclasses_s {
         name                    badgeclass_name;
         std::string             name;
         std::string             description;
         uint32_t                issued_count;
         std::string             cid_badge_image;
         std::string             cid_badge_meta;

         uint64_t primary_key() const { return badgeclass_name.value; };

      };

      typedef multi_index<"badgeclasses"_n,badgeclasses_s>     badgeclasses_t;


      /*
       * badge proposals. issuer is creating a proposal and paying
       * for RAM
       */
      TABLE proposals_s {
         uint64_t                proposal_id;
         name                    issuer;
         name                    owner;
         name                    badgeclass;

         uint64_t primary_key() const { return proposal_id; };
         uint64_t by_issuer() const { return issuer.value; };
         uint64_t by_owner() const { return owner.value; };
      };

      typedef multi_index<"proposals"_n,proposals_s,
         indexed_by<"issuer"_n, const_mem_fun<proposals_s, uint64_t, &proposals_s::by_issuer>>,
         indexed_by<"owner"_n, const_mem_fun<proposals_s, uint64_t, &proposals_s::by_owner>>>
         proposals_t;

      /*
       * A single issued badge, scoped by owner
       */
      TABLE badges_s {
         uint64_t                badge_id;
         name                    issuer;
         name                    badgeclass;

         uint64_t primary_key() const { return badge_id; };
         uint64_t by_issuer() const { return issuer.value; };
         uint64_t by_badgeclass() const { return badgeclass.value; };
      };

      typedef multi_index<"badges"_n, badges_s,
         indexed_by<"issuer"_n, const_mem_fun<badges_s, uint64_t, &badges_s::by_issuer>>,
         indexed_by<"badgeclass"_n, const_mem_fun<badges_s, uint64_t, &badges_s::by_badgeclass>>>
         badges_t;
          

      profiles_t        profiles = profiles_t(get_self(), get_self().value);
      proposals_t       proposals = proposals_t(get_self(), get_self().value);

   public:

      struct receipt_propose_s {
         uint64_t                proposal_id;
         name                    issuer;
         name                    owner;
         std::string             name;
         std::string             description;

         receipt_propose_s(const proposals_s& proposal, const badgeclasses_s& badgeclass) {
            proposal_id = proposal.proposal_id;
            issuer = proposal.issuer;
            owner = proposal.owner;
            name = badgeclass.name;
            description = badgeclass.description;
         }

         receipt_propose_s() { }
      };

      EOSLIB_SERIALIZE(receipt_propose_s, (proposal_id)(issuer)(owner)(name)(description));

      ACTION receiptissue(receipt_propose_s& receipt);
};