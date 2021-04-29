pragma solidity ^0.5.0;
pragma experimental ABIEncoderV2;

import "./math/SafeMath.sol";
import "./token/ERC20/ERC20Burnable.sol";
import "./ownership/Ownable.sol";
import "./utils/StringUtils.sol";
/**
 * @title Premium Subscription Token
 * @dev Inherit from it to add ability for your token to sell premium subscriptions
 */
contract PremiumSubscriptionToken is ERC20Burnable, Ownable, StringUtils {

    using SafeMath for uint256;

    struct PremiumSubscriptionDuration {
        uint256 openingTime;
        uint256 closingTime;
    }

    struct PremiumSubscriptionType {
        uint256 premiumSubscriptionTypeId;
        uint256 premiumSubscriptionPricePerDay;
        // You can add your own parameters here
    }

    event AddPremiumSubscriptionTypeEvent(uint256 _premiumSubscriptionTypeId);
    event EditPremiumSubscriptionTypeEvent(uint256 _premiumSubscriptionTypeId);
    event DeletePremiumSubscriptionTypeEvent(uint256 _premiumSubscriptionTypeId);

    // If we have a record here, then user with address has a subscription with PremiumSubscriptionDuration duration to certain PremiumSubscriptionType
    // string key = user address + premiumSubscriptionTypeId
    // Even if we deleted premium type, such approach will allow us to fulfill existing premium subscription until they will expire
    mapping(string => PremiumSubscriptionDuration) private premiumSubscriptionDurations;

    // Database of all available premium subscription types
    mapping(uint256 => PremiumSubscriptionType) private premiumSubscriptionTypes;

    /**
     * @dev Add new premium subscription type
     * @param _premiumSubscriptionTypeId Id of the new subscription type
     * @param _premiumSubscriptionPricePerDay Price per day of the new subscription type
     */
    function addPremiumSubscriptionType(uint256 _premiumSubscriptionTypeId, uint256 _premiumSubscriptionPricePerDay) external onlyOwner {
        PremiumSubscriptionType memory premiumSubscriptionType = PremiumSubscriptionType(_premiumSubscriptionTypeId, _premiumSubscriptionPricePerDay);
        premiumSubscriptionTypes[_premiumSubscriptionTypeId] = premiumSubscriptionType;
        emit AddPremiumSubscriptionTypeEvent(_premiumSubscriptionTypeId);
    }

    /**
     * @dev Add new premium subscription types in batch
     * @param _premiumSubscriptionTypeIds Ids of the new subscription types
     * @param _premiumSubscriptionPricesPerDay Prices per day of the new subscription types
     */
    function batchAddPremiumSubscriptionTypes(uint256[] calldata _premiumSubscriptionTypeIds, uint256[] calldata _premiumSubscriptionPricesPerDay) external onlyOwner {

        require(_premiumSubscriptionTypeIds.length == _premiumSubscriptionPricesPerDay.length);

        for (uint i=0; i<_premiumSubscriptionTypeIds.length; i++) {
            PremiumSubscriptionType memory premiumSubscriptionType = PremiumSubscriptionType(_premiumSubscriptionTypeIds[i], _premiumSubscriptionPricesPerDay[i]);
            premiumSubscriptionTypes[_premiumSubscriptionTypeIds[i]] = premiumSubscriptionType;
            emit AddPremiumSubscriptionTypeEvent(_premiumSubscriptionTypeIds[i]);
        }
    }

    /**
     * @dev Edit existing premium subscription type
     * @param _premiumSubscriptionTypeId Id of the existing subscription type
     * @param _premiumSubscriptionPricePerDay Price per day of the existing subscription type to change
     */
    function editPremiumSubscriptionType(uint256 _premiumSubscriptionTypeId, uint256 _premiumSubscriptionPricePerDay) external onlyOwner {
        PremiumSubscriptionType memory premiumSubscriptionType = PremiumSubscriptionType(_premiumSubscriptionTypeId, _premiumSubscriptionPricePerDay);
        premiumSubscriptionTypes[_premiumSubscriptionTypeId] = premiumSubscriptionType;
        emit EditPremiumSubscriptionTypeEvent(_premiumSubscriptionTypeId);
    }

    /**
     * @dev Edit existing premium subscription types in batch
     * @param _premiumSubscriptionTypeIds Ids of the existing subscription types
     * @param _premiumSubscriptionPricesPerDay Prices per day of the existing subscription types to change
     */
    function batchEditPremiumSubscriptionTypes(uint256[] calldata _premiumSubscriptionTypeIds, uint256[] calldata _premiumSubscriptionPricesPerDay) external onlyOwner {

        require(_premiumSubscriptionTypeIds.length == _premiumSubscriptionPricesPerDay.length);

        for (uint i=0; i<_premiumSubscriptionTypeIds.length; i++) {
            PremiumSubscriptionType memory premiumSubscriptionType = PremiumSubscriptionType(_premiumSubscriptionTypeIds[i], _premiumSubscriptionPricesPerDay[i]);
            premiumSubscriptionTypes[_premiumSubscriptionTypeIds[i]] = premiumSubscriptionType;
            emit EditPremiumSubscriptionTypeEvent(_premiumSubscriptionTypeIds[i]);
        }
    }

    /**
     * @dev Delete existing premium subscription type
     * @param _premiumSubscriptionTypeId Id of the existing subscription type
     */
    function deletePremiumSubscriptionType(uint256 _premiumSubscriptionTypeId) external onlyOwner {
        delete premiumSubscriptionTypes[_premiumSubscriptionTypeId];
        emit DeletePremiumSubscriptionTypeEvent(_premiumSubscriptionTypeId);
    }

    /**
     * @dev Delete existing premium subscription types in batch
     * @param _premiumSubscriptionTypeIds Ids of the existing subscription types
     */
    function batchDeletePremiumSubscriptionTypes(uint256[] calldata _premiumSubscriptionTypeIds) external onlyOwner {
        for (uint i=0; i<_premiumSubscriptionTypeIds.length; i++) {
            delete premiumSubscriptionTypes[_premiumSubscriptionTypeIds[i]];
            emit DeletePremiumSubscriptionTypeEvent(_premiumSubscriptionTypeIds[i]);
        }
    }

    /**
     * @dev Get the price of existing premium subscription type
     * @param _premiumSubscriptionTypeId Id of the existing subscription type
     * @return Price of the existing subscription type
     */
    function getPremiumSubscriptionPricePerDay(uint256 _premiumSubscriptionTypeId) public view returns (uint256) {
        return premiumSubscriptionTypes[_premiumSubscriptionTypeId].premiumSubscriptionPricePerDay;
    }

    /**
     * @dev Buy premium subscription type with selected Id for N amount of days
     * @param _days Amount of days, when subscription will be active
     * @param _premiumSubscriptionTypeId Id of the subscription type, which user wants to buy
     */
    function buyPremiumSubscription(uint _days, uint256 _premiumSubscriptionTypeId) external {

        // We should have such premium subscription type
        require(premiumSubscriptionTypes[_premiumSubscriptionTypeId].premiumSubscriptionTypeId != 0);

        // User should have sufficient amount of tokens
        require(balanceOf(msg.sender) >= _days * premiumSubscriptionTypes[_premiumSubscriptionTypeId].premiumSubscriptionPricePerDay, "Error: insufficient funds");

        // User should not have active subscription (to not overwrite it)
        require(!checkPremiumSubscription(msg.sender, _premiumSubscriptionTypeId), "Error: premium subscription is still active");

        // Burn user tokens
        burn(_days * premiumSubscriptionTypes[_premiumSubscriptionTypeId].premiumSubscriptionPricePerDay);

        // Grant subscription
        PremiumSubscriptionDuration memory premiumSubscriptionDuration = PremiumSubscriptionDuration(now, now + (_days * 1 days));

        string memory key = string(abi.encodePacked(msg.sender, uintToString(_premiumSubscriptionTypeId)));
        premiumSubscriptionDurations[key] = premiumSubscriptionDuration;
    }

    /**
     * @dev Grant premium subscription type with selected Id for N amount of days to certain user
     * @param _user Wallet address of the user
     * @param _days Amount of days, when subscription will be active
     * @param _premiumSubscriptionTypeId Id of the subscription type, which admin wants to grant
     */
    function grantPremiumSubscription(address _user, uint _days, uint256 _premiumSubscriptionTypeId) external onlyOwner {

        // We should have such premium subscription type
        require(premiumSubscriptionTypes[_premiumSubscriptionTypeId].premiumSubscriptionTypeId != 0);

        // User should not have active subscription (to not overwrite it)
        require(!checkPremiumSubscription(_user, _premiumSubscriptionTypeId), "Error: premium subscription is still active");

        // Grant subscription
        PremiumSubscriptionDuration memory premiumSubscriptionDuration = PremiumSubscriptionDuration(now, now + (_days * 1 days));

        string memory key = string(abi.encodePacked(_user, uintToString(_premiumSubscriptionTypeId)));
        premiumSubscriptionDurations[key] = premiumSubscriptionDuration;
    }

    /**
     * @dev Check premium subscription
     * @param _user Wallet address of the user
     * @param _premiumSubscriptionTypeId Id of the existing subscription type
     * @return true if user have active subscription or false if not
     */
    function checkPremiumSubscription(address _user, uint256 _premiumSubscriptionTypeId) public view returns (bool) {
        string memory key = string(abi.encodePacked(_user, uintToString(_premiumSubscriptionTypeId)));
        return block.timestamp >= premiumSubscriptionDurations[key].openingTime && block.timestamp <= premiumSubscriptionDurations[key].closingTime;
    }

}
