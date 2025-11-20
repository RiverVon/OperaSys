import hashlib
import time
import json

# --- 基础 Block 类 (同任务一) ---
class Block:
    def __init__(self, index, previous_hash, data):
        self.index = index
        self.timestamp = time.time()
        self.data = data
        self.previous_hash = previous_hash
        self.hash = self.calculate_hash()

    def calculate_hash(self):
        # 将 Block 对象转为字典，排除不能序列化的字段（如果有的话）
        block_data = {
            "index": self.index,
            "timestamp": self.timestamp,
            "data": self.data,
            "previous_hash": self.previous_hash
        }
        block_string = json.dumps(block_data, sort_keys=True).encode()
        return hashlib.sha256(block_string).hexdigest()

# --- 智能合约类 ---
class SmartContract:
    def __init__(self, creator, beneficiary, amount):
        self.creator = creator
        self.beneficiary = beneficiary
        self.amount = amount
        self.state = "created" # 初始状态

    def execute(self):
        # 模拟合约执行逻辑
        if self.state == "created":
            self.state = "executed"
            return True
        return False

    def get_status(self):
        return self.state

# --- 扩展的 Blockchain 类 ---
class Blockchain:
    def __init__(self):
        self.chain = []
        # 创建创世区块，显式传递 previous_hash
        self.create_block(data="Genesis Block", previous_hash="0")

    def create_block(self, data, previous_hash=None):
        # 自动处理哈希链接
        if previous_hash is None:
            if len(self.chain) > 0:
                previous_block = self.chain[-1]
                previous_hash = previous_block.hash
            else:
                previous_hash = "0"
        
        index = len(self.chain)
        block = Block(index, previous_hash, data)
        self.chain.append(block)
        self.save_block_to_file(block)
        return block

    def save_block_to_file(self, block):
        # 将复杂的数据对象转为字符串存储
        data_str = json.dumps(block.data) if isinstance(block.data, dict) else str(block.data)
        with open('blockchain_v2.txt', 'a') as f:
            f.write(f"{block.index} | {block.timestamp} | {data_str} | {block.hash} | {block.previous_hash}\n")

    def print_chain(self):
        for block in self.chain:
            print(f"Index: {block.index}\n Data: {block.data}\n Hash: {block.hash}\n Prev: {block.previous_hash}\n" + "-"*20)

    # --- 智能合约相关方法 ---
    def create_smart_contract(self, creator, beneficiary, amount):
        contract = SmartContract(creator, beneficiary, amount)
        # 将合约创建事件记录到区块链
        contract_data = {
            "action": "CREATE_CONTRACT",
            "creator": contract.creator,
            "beneficiary": contract.beneficiary,
            "amount": contract.amount,
            "state": contract.state
        }
        self.create_block(contract_data)
        return contract

    def execute_contract(self, contract):
        if contract.execute():
            # 将合约执行事件记录到区块链
            contract_data = {
                "action": "EXECUTE_CONTRACT",
                "creator": contract.creator,
                "beneficiary": contract.beneficiary,
                "amount": contract.amount,
                "state": contract.state
            }
            self.create_block(contract_data)
            return True
        return False

# --- 实验执行部分 ---
if __name__ == "__main__":
    print("--- Starting Extended Blockchain with Smart Contracts ---")
    blockchain = Blockchain()

    # 1. 创建智能合约
    print("\n[Action] Creating Smart Contract (Alice -> Bob: 100)...")
    contract = blockchain.create_smart_contract("Alice", "Bob", 100)
    
    # 2. 打印链状态 (查看合约创建记录)
    print("\n[Status] Chain after contract creation:")
    blockchain.print_chain()

    # 3. 执行合约
    print("\n[Action] Executing Contract...")
    if blockchain.execute_contract(contract):
        print(">> Contract executed successfully!")
    else:
        print(">> Contract execution failed!")

    # 4. 再次打印链状态 (查看合约执行记录)
    print("\n[Status] Chain after execution:")
    blockchain.print_chain()