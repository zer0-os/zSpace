pragma solidity ^0.5.0;

import "./math/SafeMath.sol";
import "./token/ERC20/ERC20Burnable.sol";
import "./ownership/Ownable.sol";
import "./utils/StringUtils.sol";

/**
 * @title Premium Item Token
 * @dev Inherit from it to add ability for your token to sell premium items
 */
contract PremiumItemToken is ERC20Burnable, Ownable {

    using SafeMath for uint256;

    struct PremiumItem {
        uint256 itemId;
        uint256 itemPrice;
        // You can add your own parameters here
    }

    event AddPremiumItemEvent(uint256 _itemId);
    event EditPremiumItemEvent(uint256 _itemId);
    event DeletePremiumItemEvent(uint256 _itemId);

    // If we have a record here, then user with address owns item with itemId
    mapping(address => uint256) private premiumItemOwners;

    // Database of all available items
    mapping(uint256 => PremiumItem) private premiumItems;

    /**
     * @dev Add new premium item
     * @param _itemId Id of the new item
     * @param _itemPrice Price of the new item
     */
    function addPremiumItem(uint256 _itemId, uint256 _itemPrice) external onlyOwner {
        PremiumItem memory premiumItem = PremiumItem(_itemId, _itemPrice);
        premiumItems[_itemId] = premiumItem;
        emit AddPremiumItemEvent(_itemId);
    }

    /**
     * @dev Add new premium items in batch
     * @param _itemIds Ids of the new items
     * @param _itemPrices Prices of the new items
     */
    function batchAddPremiumItems(uint256[] calldata _itemIds, uint256[] calldata _itemPrices) external onlyOwner {

        require(_itemIds.length == _itemPrices.length);

        for (uint i=0; i<_itemIds.length; i++) {
            PremiumItem memory premiumItem = PremiumItem(_itemIds[i], _itemPrices[i]);
            premiumItems[_itemIds[i]] = premiumItem;
            emit AddPremiumItemEvent(_itemIds[i]);
        }

    }

    /**
     * @dev Edit premium item
     * @param _itemId Id of the existing item
     * @param _itemPrice Price of the existing item to change
     */
    function editPremiumItem(uint256 _itemId, uint256 _itemPrice) external onlyOwner {
        PremiumItem memory premiumItem = PremiumItem(_itemId, _itemPrice);
        premiumItems[_itemId] = premiumItem;
        emit EditPremiumItemEvent(_itemId);
    }

    /**
     * @dev Edit new premium items in batch
     * @param _itemIds Ids of the existing items
     * @param _itemPrices Prices of the existing items to change
     */
    function batchEditPremiumItems(uint256[] calldata _itemIds, uint256[] calldata _itemPrices) external onlyOwner {

        require(_itemIds.length == _itemPrices.length);

        for (uint i=0; i<_itemIds.length; i++) {
            PremiumItem memory premiumItem = PremiumItem(_itemIds[i], _itemPrices[i]);
            premiumItems[_itemIds[i]] = premiumItem;
            emit EditPremiumItemEvent(_itemIds[i]);
        }

    }

    /**
     * @dev Delete existing premium item
     * @param _itemId Id of the item, which should be deleted
     */
    function deletePremiumItem(uint256 _itemId) external onlyOwner {
        delete premiumItems[_itemId];
        emit DeletePremiumItemEvent(_itemId);
    }

    /**
     * @dev Delete existing premium items in batch
     * @param _itemIds Ids of the items, which should be deleted
     */
    function batchDeletePremiumItems(uint256[] calldata _itemIds) external onlyOwner {
        for (uint i=0; i<_itemIds.length; i++) {
            delete premiumItems[_itemIds[i]];
            emit DeletePremiumItemEvent(_itemIds[i]);
        }
    }

    /**
     * @dev Get the price of existing premium item
     * @param _itemId Id of the existing item
     * @return Price of the existing item
     */
    function getPremiumItemPrice(uint256 _itemId) public view returns (uint256) {
        return premiumItems[_itemId].itemPrice;
    }

    /**
     * @dev Buy premium item with selected Id
     * @param _itemId Id of the item, which user wants to buy
     */
    function buyPremiumItem(uint256 _itemId) external {

        // We should have such premium item
        require(premiumItems[_itemId].itemId != 0);

        // User should have sufficient amount of tokens
        require(balanceOf(msg.sender) >= premiumItems[_itemId].itemPrice, "Error: insufficient funds");

        // User should not posess this item
        require(!checkPremiumItem(msg.sender, _itemId), "Error: premium item already owned");

        // Burn user tokens
        burn(premiumItems[_itemId].itemPrice);

        // Grant item
        premiumItemOwners[msg.sender] = _itemId;
    }

    /**
     * @dev Grant premium item with selected Id to certain user
     * @param _user Wallet address of the user
     * @param _itemId Id of the item, which admin wants to grant
     */
    function grantPremiumItem(address _user, uint256 _itemId) external onlyOwner {

        // We should have such premium item
        require(premiumItems[_itemId].itemId != 0);

        // Grant item
        premiumItemOwners[_user] = _itemId;
    }

    /**
     * @dev Check premium item
     * @param _user Wallet address of the user
     * @param _itemId Id of the existing item
     * @return true if user have such item or false if not
     */
    function checkPremiumItem(address _user, uint256 _itemId) public view returns (bool) {
        return premiumItemOwners[_user] == _itemId;
    }

}
