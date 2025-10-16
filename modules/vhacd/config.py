def can_build(env, platform):
    # 在禁用 3D 或禁用 3D 物理时不构建该模块
    return not env.get("disable_3d", False) and not env.get("disable_physics_3d", False)


def configure(env):
    pass
