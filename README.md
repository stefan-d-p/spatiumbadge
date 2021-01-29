# Badge Smart Contract for eos.io

Under construction!

### signup(const name& issuer, const std::string& org_name, const std::string& url, const std::string& cid_image)

Someone who wants to issue badges (awards) to other accounts needs first to signup and provide an organisation name, an url and an ipfs cid for the logo.

profile table scoped to contract. issuer is paying for ram.

### createclass(const name& issuer, const name& badgeclass_name, const std::string& name, const std::string& description, const std::string& cid_image, const std::string& cid_meta)

After signing up an issuer can create one or more badgeclasses. Badgeclasses are templates consiting of a name (eg. Certified eos.io Master), a description (eg. showed excellence in mastering the eos.io platform), an ipfs image cid for the badge image and an ipfs dag cid containing additional information.

badgeclasses table scoped to issuer. issuer is paying for ram.

### issue(const name& issuer, const name& owner, const name& badgeclass, const std::string& memo)

based on a badgeclass an issuer can then propose a badge to another account, providing the account, the badgeclass_name and a memo.

proposal table scoped to contract. issuer is paying for ram

### accept(uint64_t& proposal_id)

the receiver of a proposal needs to accept a proposal providing the proposal_id. When accepting the actual badge is created and the proposal entry is erased.

badge table is scoped to owner (receiver). the owner is paying for ram.

### decline(uint64_t& proposal_id)

the receiver of a proposal can decline a proposal proving the proposal id. The proposal is removed from the proposals table.