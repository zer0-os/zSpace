pragma solidity ^0.5.0;

import "./math/SafeMath.sol";
import "./ownership/Ownable.sol";
import "./token/ERC721/ERC721Mintable.sol";
import "./token/ERC721/ERC721Full.sol";


/**
 * @title EtherPet Token
 * @dev Simple ERC721 token example
 */
contract EtherPetToken is ERC721, ERC721Enumerable, ERC721Mintable, Ownable {

    using SafeMath for uint256;

    event NewEntity(uint entityId, string name, uint dna);
    event Withdrawn(uint256 weiAmount);

    string public constant name = "EtherPet";
    string public constant symbol = "EP";
    uint8 public constant decimals = 0;

    uint dnaDigits = 16;
    uint dnaModulus = 10 ** dnaDigits;

    uint256 private randomEntityPrice = 1000000000000000;

    // Entity structure
    struct Entity {
        string name;
        uint256 dna;
        // You can add your own parameters here
    }

    // entities data
    Entity[] public entities;

    /**
     * @dev Get entity name
     * @param _entityId Entity Id
     * @return name of an entity
     */
    function getEntityName(uint256 _entityId) public view returns(string memory) {

       Entity memory _entity = entities[_entityId];
       return _entity.name;
    }

    /**
     * @dev Get entity DNA
     * @param _entityId Entity Id
     * @return DNA of an entity
     */
    function getEntityDNA(uint256 _entityId) public view returns(uint256) {

        Entity memory _entity = entities[_entityId];
        return _entity.dna;
    }

    /**
     * @dev Buy random entity
     * @param _name Entity Name
     */
    function buyRandomEntity(string calldata _name) external payable {

        // User should pay sufficient amount of funds and no more than that
        require(msg.value == randomEntityPrice);

        uint randDna = _generateRandomDna(_name);
        randDna = randDna - randDna % 100;

        _createEntity(msg.sender, _name, randDna);

    }

    /**
     * @dev Grant random entity (can be used only by owner of contract)
     * @param _entityOwner Entity Owner
     * @param _name Entity Name
     */
    function grantRandomEntity(address _entityOwner, string calldata _name) external onlyOwner {

        uint randDna = _generateRandomDna(_name);
        randDna = randDna - randDna % 100;

        _createEntity(_entityOwner, _name, randDna);
    }

    /**
     * @dev Create random entity, if sender doesn't have any entities
     * @param _name Entity Name
     */
    function createFreeRandomEntity(string calldata _name) external {

        // Only first entity is free
        require(balanceOf(msg.sender) == 0);

        uint randDna = _generateRandomDna(_name);
        randDna = randDna - randDna % 100;

        _createEntity(msg.sender, _name, randDna);
    }

    /**
     * @dev Create a new entity
     * @param _entityOwner Entity Owner
     * @param _name Entity Name
     * @param _dna Entity DNA
     */
    function _createEntity(address _entityOwner, string memory _name, uint256 _dna) internal {

        uint entityId = entities.push(Entity(_name, _dna)) - 1;

        _mint(_entityOwner, entityId);

        emit NewEntity(entityId, _name, _dna);
    }

    /**
    * @dev Generate random DNA for the entity
    * @param _name Entity Name
    * @return randomDNA Random DNA of a entity
    */
    function _generateRandomDna(string memory _name) private view returns (uint256) {
        uint256 rand = uint256(keccak256(abi.encode(_name)));
        return rand % dnaModulus;
    }

    /**
     * @dev Get price of the random entity
     * @return randomEntityPrice random entity price in Wei
     */
    function getRandomEntityPrice() public view returns(uint256) {
        return randomEntityPrice;
    }

    /**
     * @dev Set random entity price in Wei
     * @param _price New Price
     */
    function setRandomEntityPrice(uint256 _price) external onlyOwner {
        randomEntityPrice = _price;
    }

    /**
     * @dev Withdrawn funds from selling random entities, to the owner of a contract
     */
    function withdrawFunds() external onlyOwner {
        uint256 balance = address(this).balance;
        address payable ownerPayableAddress = ownerPayable();
        ownerPayableAddress.transfer(address(this).balance);
        emit Withdrawn(balance);
    }

}
