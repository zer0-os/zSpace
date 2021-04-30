pragma solidity ^0.5.0;

import "./math/SafeMath.sol";
import "./token/ERC20/ERC20Mintable.sol";
import "./token/ERC20/ERC20Burnable.sol";
import "./PremiumSubscriptionToken.sol";
import "./PremiumItemToken.sol";

/**
 * @title EtherGald Token
 * @dev Simple ERC20 token example with ability to sell premium items and subscriptions and withdrawn funds
 */
contract EtherGaldToken is ERC20Mintable, PremiumSubscriptionToken, PremiumItemToken {

    using SafeMath for uint256;

    string public constant name = "EtherGald";
    string public constant symbol = "EG";
    uint8 public constant decimals = 0;

    uint256 private tokenPrice = 1000000000000000;

    event Withdrawn(uint256 weiAmount);

    /**
     * @dev Withdrawn funds from selling tokens, premium accounts and perks, to the owner of a contract
     */
    function withdrawFunds() public onlyOwner {
        uint256 balance = address(this).balance;
        address payable ownerPayableAddress = ownerPayable();
        ownerPayableAddress.transfer(address(this).balance);
        emit Withdrawn(balance);
    }

    /**
     * @dev Get price of the token
     * @return tokenPrice token price in Wei
     */
    function getTokenPrice() public view returns(uint256) {
        return tokenPrice;
    }

    /**
     * @dev Set token price in Wei
     * @param _price New Price
     */
    function setTokenPrice(uint256 _price) external onlyOwner {
        tokenPrice = _price;
    }

    /**
     * @dev Buy tokens, proportionally to the ETH amount, which user paid
     */
    function buyToken() external payable {
        require(msg.value.mod(tokenPrice) == 0);
        uint256 amount = msg.value.div(tokenPrice);
        require(amount >= 1);
        _mint(msg.sender, amount);
    }

    /**
     * @dev Check token balance
     * @return token balance of sender
     */
    function checkTokenBalance() public view returns (uint256) {
        return balanceOf(msg.sender);
    }

}
