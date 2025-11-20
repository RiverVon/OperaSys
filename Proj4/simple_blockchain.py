import hashlib
import time
import json

class Block:
    def __init__(self, index, previous_hash, data):
        self.index = index
        self.timestamp = time.time()
        self.data = data
        self.previous_hash = previous_hash
        self.hash = self.calculate_hash()

    def calculate_hash(self):
        # 将区块信息转换为 JSON 字符串并编码
        block_string = json.dumps(self.__dict__, sort_keys=True).encode()
        # 使用 SHA256 算法计算哈希
        return hashlib.sha256(block_string).hexdigest()

class Blockchain:
    def __init__(self):
        self.chain = []
        # 创建创世区块 (Genesis Block)
        self.create_block(data="Genesis Block", previous_hash="0")

    def create_block(self, data, previous_hash=None):
        # 如果没有指定前一个哈希，则自动获取链末尾区块的哈希
        if previous_hash is None:
            previous_block = self.chain[-1]
            previous_hash = previous_block.hash
        
        index = len(self.chain)
        block = Block(index, previous_hash, data)
        
        self.chain.append(block)
        self.save_block_to_file(block) # 关键点：文件存储
        return block

    def save_block_to_file(self, block):
        # 这里模拟文件系统操作，将数据持久化
        with open('blockchain.txt', 'a') as f:
            f.write(f"{block.index}, {block.timestamp}, {block.data}, {block.hash}, {block.previous_hash}\n")

    def print_chain(self):
        for block in self.chain:
            print(f"Index: {block.index}, Timestamp: {block.timestamp}, Data: {block.data}, Hash: {block.hash}, Prev Hash: {block.previous_hash}")

# --- 实验执行部分 ---
if __name__ == "__main__":
    # 1. 初始化区块链
    print("Initializing Blockchain...")
    blockchain = Blockchain()

    # 2. 添加新区块
    print("Mining block 1...")
    blockchain.create_block("First block data")

    print("Mining block 2...")
    blockchain.create_block("Second block data")

    # 3. 打印链状态
    print("\nCurrent Blockchain Status:")
    blockchain.print_chain()
    print("\nData has been saved to blockchain.txt")