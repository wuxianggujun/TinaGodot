# Implementation Plan - Remove Animation Editor

## Task List

- [x] 1. Update build system configuration


  - Comment out animation directory build reference in editor/SCsub
  - Add clear comment explaining the removal
  - _Requirements: 3.1, 3.4_

- [x] 2. Update editor_node.cpp to remove animation editor references


  - [x] 2.1 Comment out animation_player_editor_plugin.h header include


    - Locate line 74 with the include statement
    - Comment out with TinaFlowStudio removal comment
    - _Requirements: 3.2, 1.2_

  - [x] 2.2 Remove AnimationPlayerEditor singleton usage in scene reimport


    - Locate lines 1319-1322 with AnimationPlayerEditor::get_singleton() call
    - Comment out the animation stop logic
    - Add comment explaining removal
    - _Requirements: 3.2, 1.2_



  - [x] 2.3 Remove animation editor plugin registrations


    - Locate lines 8485-8487 with add_editor_plugin calls
    - Comment out AnimationPlayerEditorPlugin registration
    - Comment out AnimationTrackKeyEditEditorPlugin registration


    - Comment out AnimationMarkerKeyEditEditorPlugin registration
    - _Requirements: 1.2, 3.2_





  - [ ] 2.4 Remove bottom panel animation editor management
    - Locate line 8505 with bottom_panel->move_item_to_end call


    - Comment out the AnimationPlayerEditor bottom panel code
    - _Requirements: 1.1, 3.2_




- [ ] 3. Update register_editor_types.cpp to remove animation plugin registration
  - [ ] 3.1 Comment out animation_tree_editor_plugin.h header include
    - Locate line 34 with the include statement
    - Comment out with TinaFlowStudio removal comment
    - _Requirements: 3.3, 1.2_

  - [ ] 3.2 Remove AnimationTreeEditorPlugin registration
    - Locate line 185 with EditorPlugins::add_by_type call
    - Comment out the plugin registration
    - _Requirements: 1.2, 3.3_

- [ ] 3.3 Fix additional AnimationPlayer header include in editor_node.cpp
  - Add missing #include "scene/animation/animation_player.h" for runtime AnimationPlayer class
  - This is needed for code that uses AnimationPlayer at line 2092
  - _Requirements: 1.5, 3.5_

- [ ] 3.4 Remove AnimationPlayerEditor references from scene_tree_editor.cpp
  - Comment out AnimationPlayerEditor::get_singleton() calls in BUTTON_PIN handler
  - Comment out animation mixer pinning logic in _update_node
  - Comment out pinned node tracking in update loop
  - _Requirements: 1.2, 3.5_

- [ ] 3.5 Remove AnimationTrackEditor references from canvas_item_editor_plugin.cpp
  - Comment out AnimationTrackEditor usage in _keying_changed function
  - Comment out animation editor signal connections in NOTIFICATION_ENTER_TREE
  - Disable _insert_animation_keys function body
  - _Requirements: 1.2, 3.5_

- [ ] 3.6 Remove AnimationTrackEditor references from scene_tree_dock.cpp
  - Comment out AnimationTrackEditor::get_root() checks in _do_reparent
  - Comment out animation track editor root updates in undo/redo operations
  - _Requirements: 1.2, 3.5_

- [x] 4. Verify build compilation

  - [x] 4.1 Perform clean build

    - Run scons --clean to remove previous build artifacts
    - Run scons platform=windows target=editor to build
    - Verify build completes without errors
    - _Requirements: 1.5, 3.4, 3.5_

  - [x] 4.2 Check for linker errors

    - Review build output for undefined reference errors
    - Ensure no animation editor symbols are missing
    - _Requirements: 3.5_

- [x] 5. Delete animation editor source directory


  - Delete the entire editor/animation/ directory (20 files)
  - Verify deletion includes all .cpp, .h, and SCsub files
  - _Requirements: 1.3_

- [x] 6. Verify build after directory deletion



  - Run scons platform=windows target=editor again
  - Confirm build still completes successfully
  - _Requirements: 1.5, 3.4, 3.5_

- [ ] 7. Test editor functionality
  - [ ] 7.1 Test editor startup
    - Launch the editor
    - Verify no crashes occur
    - Verify no error messages about missing animation components
    - Check that bottom panel does not show "Animation" tab
    - _Requirements: 4.1, 4.2, 1.1_

  - [ ] 7.2 Test AnimationPlayer node functionality
    - Create a new scene
    - Add an AnimationPlayer node
    - Verify node can be added successfully
    - Verify node properties are visible in inspector
    - _Requirements: 2.1, 4.3_

  - [ ] 7.3 Test animation runtime playback
    - Create a simple scene with AnimationPlayer
    - Add a pre-made animation resource
    - Run the scene
    - Verify animation plays correctly at runtime
    - _Requirements: 2.1, 2.3, 4.4_

  - [ ] 7.4 Test scene reimport functionality
    - Import a scene file
    - Modify and reimport the scene
    - Verify reimport completes without errors
    - Verify no crashes occur
    - _Requirements: 4.2_

  - [ ] 7.5 Test AnimationTree node functionality
    - Add AnimationTree node to a scene
    - Verify node can be added
    - Verify runtime functionality is preserved
    - _Requirements: 2.2, 2.4_

- [ ] 8. Measure and document results
  - [ ] 8.1 Record binary size reduction
    - Compare editor binary size before and after removal
    - Document the size reduction achieved
    - _Requirements: 4.5_

  - [x] 8.2 Update TINAFLOWSTUDIO.md documentation


    - Mark animation editor removal task as complete
    - Add statistics: 20 files removed, ~900KB source code
    - Update the 精简统计 table with new totals
    - Document what was removed vs. preserved
    - Update current phase status
    - _Requirements: 1.1, 1.3_

  - [-] 8.3 Create git commit

    - Commit all changes with clear message
    - Use format: [TinaFlowStudio] Remove animation editor UI and plugins
    - Include summary of changes in commit description
    - _Requirements: 1.5_
