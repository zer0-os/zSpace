pragma solidity ^0.5.0;

import "./math/SafeMath.sol";
import "./ownership/Ownable.sol";

/**
 * @title Donations contract
 * @dev Can be used to collect donations for your projects
 */
contract Donations is Ownable {

    using SafeMath for uint256;

    // Contains amount of donations per sender
    mapping (address => uint256) private donations;

    event Deposited(address indexed payer, uint256 weiAmount);
    event Withdrawn(uint256 weiAmount);

    /**
     * @dev Withdraw available funds from this contract
     */
    function withdrawDonations() public onlyOwner {
        uint256 balance = address(this).balance;
        address payable ownerPayableAddress = ownerPayable();
        ownerPayableAddress.transfer(address(this).balance);
        emit Withdrawn(balance);
    }

    /**
     * @dev Donate ETH to the contract
     */
    function donate() public payable {
        uint256 amount = msg.value;
        donations[msg.sender] = donations[msg.sender].add(amount);
        emit Deposited(msg.sender, amount);
    }

    /**
     * @dev Get contract balance
     * @return Balance of the contract
     */
    function getBalance() public view returns (uint256) {
        return address(this).balance;
    }

    /**
     * @dev Check donations for specific user
     * @param user Address of the user, which donated funds
     * @return How much user donated ETH to this contract
     */
    function checkDonations(address user) public view returns (uint256) {
        return donations[user];
    }
}