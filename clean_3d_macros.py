#!/usr/bin/env python3
import re
import os
import sys

def remove_3d_macro_blocks(content):
    """删除所有被3D宏包裹的代码块"""
    # 删除 #ifndef _3D_DISABLED ... #endif 块
    content = re.sub(r'#ifndef\s+_3D_DISABLED\s*\n.*?#endif\s*//\s*_3D_DISABLED', '', content, flags=re.DOTALL)
    content = re.sub(r'#ifndef\s+_3D_DISABLED\s*\n.*?#endif', '', content, flags=re.DOTALL)
    
    # 删除 #ifndef PHYSICS_3D_DISABLED ... #endif 块
    content = re.sub(r'#ifndef\s+PHYSICS_3D_DISABLED\s*\n.*?#endif\s*//\s*PHYSICS_3D_DISABLED', '', content, flags=re.DOTALL)
    content = re.sub(r'#ifndef\s+PHYSICS_3D_DISABLED\s*\n.*?#endif', '', content, flags=re.DOTALL)
    
    # 删除 #ifndef NAVIGATION_3D_DISABLED ... #endif 块
    content = re.sub(r'#ifndef\s+NAVIGATION_3D_DISABLED\s*\n.*?#endif\s*//\s*NAVIGATION_3D_DISABLED', '', content, flags=re.DOTALL)
    content = re.sub(r'#ifndef\s+NAVIGATION_3D_DISABLED\s*\n.*?#endif', '', content, flags=re.DOTALL)
    
    # 删除 #ifndef XR_DISABLED ... #endif 块
    content = re.sub(r'#ifndef\s+XR_DISABLED\s*\n.*?#endif\s*//\s*XR_DISABLED', '', content, flags=re.DOTALL)
    content = re.sub(r'#ifndef\s+XR_DISABLED\s*\n.*?#endif', '', content, flags=re.DOTALL)
    
    # 删除多余的空行
    content = re.sub(r'\n\n\n+', '\n\n', content)
    
    return content

def process_file(filepath):
    """处理单个文件"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        content = remove_3d_macro_blocks(content)
        
        if content != original_content:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"[OK] Cleaned: {filepath}")
            return True
        else:
            print(f"[SKIP] {filepath} (no changes)")
            return False
    except Exception as e:
        print(f"[ERROR] {filepath}: {e}")
        return False

def main():
    # 要处理的文件列表
    files = [
        "core/config/project_settings.cpp",
        "editor/animation/animation_player_editor_plugin.cpp",
        "editor/animation/animation_track_editor.h",
        "editor/debugger/script_editor_debugger.cpp",
        "editor/docks/filesystem_dock.cpp",
        "editor/editor_interface.cpp",
        "editor/editor_node.cpp",
        "editor/plugins/editor_plugin.h",
        "editor/project_manager/project_manager.cpp",
        "editor/scene/scene_create_dialog.cpp",
        "editor/scene/scene_tree_editor.cpp",
        "editor/scene/sprite_frames_editor_plugin.cpp",
        "editor/settings/editor_settings_dialog.cpp",
        "editor/shader/visual_shader_editor_plugin.cpp",
        "editor/shader/visual_shader_editor_plugin.h",
        "main/main.cpp",
        "main/performance.cpp",
        "main/performance.h",
        "scene/2d/tile_map_layer.cpp",
        "scene/animation/root_motion_view.cpp",
        "scene/main/scene_tree.cpp",
        "scene/main/scene_tree.h",
        "scene/main/scene_tree_fti.cpp",
        "scene/main/scene_tree_fti.h",
        "scene/main/scene_tree_fti_tests.cpp",
        "scene/main/viewport.cpp",
        "scene/resources/packed_scene.cpp",
        "servers/register_server_types.cpp",
        "servers/rendering/renderer_scene_cull.cpp",
        "servers/rendering/renderer_viewport.cpp",
        "tests/scene/test_text_edit.h",
        "tests/test_main.cpp",
    ]
    
    base_dir = os.path.dirname(os.path.abspath(__file__))
    cleaned_count = 0
    
    for file in files:
        filepath = os.path.join(base_dir, file)
        if os.path.exists(filepath):
            if process_file(filepath):
                cleaned_count += 1
        else:
            print(f"! File not found: {filepath}")
    
    print(f"\n完成！清理了 {cleaned_count} 个文件")

if __name__ == "__main__":
    main()
