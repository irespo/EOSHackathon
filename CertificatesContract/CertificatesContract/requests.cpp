#include <eosiolib/eosio.hpp>

using namespace eosio;
using namespace std;

class requests : public contract {
	using contract::contract;
public:
	requests(action_name self) : contract(self)
		, _requests(_self, _self) {}

	//@abi table 
	struct request {
		uint64_t requestId;
		name requestor;
		string email;
		uint64_t certificateId;

		uint64_t primary_key() const { return requestId; }

		EOSLIB_SERIALIZE(request, (requestId)(requestor)(email)(certificateId))
	};

	multi_index<N(request), request> _requests;

	//@abi action
	void addrequest(uint64_t requestId, name requestor, string email, uint64_t certificateId) {
		require_auth(requestor);

		auto iterator = _requests.find(requestId);
		eosio_assert(iterator == _requests.end(), "Request with given Id already exists");

		_requests.emplace(requestor, [&](auto& row) {
			row.requestId = requestId;
			row.requestor = requestor;
			row.email = email;
			row.certificateId = certificateId;
		});

		print("Request added");
	}

	//@abi action
	void delrequest(uint64_t requestId, name requestor) {
		require_auth(requestor);

		auto iterator = _requests.find(requestId);
		eosio_assert(iterator != _requests.end(), "Request with given Id does not exist.");

		_requests.erase(iterator);

		print("Request deleted");

	}
};

EOSIO_ABI(requests, (addrequest)(delrequest))