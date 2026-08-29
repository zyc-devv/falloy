import os
import json

def get_directory_structure(path):
    """
    递归获取目录结构
    """
    structure = {
        "name": os.path.basename(path) or path,
        "type": "directory",
        "children": []
    }
    
    try:
        entries = os.listdir(path)
    except PermissionError:
        structure["error"] = "Permission denied"
        return structure
    except Exception as e:
        structure["error"] = str(e)
        return structure
    
    for entry in sorted(entries):  # 排序使输出更整洁
        full_path = os.path.join(path, entry)
        if os.path.isdir(full_path):
            # 递归处理子目录
            child = get_directory_structure(full_path)
        else:
            # 处理文件
            child = {
                "name": entry,
                "type": "file",
                # "size": os.path.getsize(full_path)
            }
        structure["children"].append(child)
    
    return structure

def main():
    # 默认搜索当前目录，也可以指定其他路径
    target_path = "."
    
    print(f"正在扫描目录: {os.path.abspath(target_path)}")
    directory_tree = get_directory_structure(target_path)
    
    # 生成格式化的 JSON 字符串
    json_output = json.dumps(directory_tree, indent=2, ensure_ascii=False)
    
    # 打印到控制台
    print(json_output)
    
    # 同时保存到文件
    output_file = "directory_structure.json"
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(json_output)
    
    print(f"\n✅ 目录结构已保存到: {output_file}")

if __name__ == "__main__":
    main()