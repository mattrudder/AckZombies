const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "ack-zombies",
        .root_source_file = null,
        .target = target,
        .optimize = optimize,
    });
    exe.linkLibC();
    // exe.linkLibCpp();
    // exe.defineCMacro("WIN32", null);
    // exe.defineCMacro("_WINDOWS", null);
    exe.addIncludePath(b.path("Inc"));
    exe.addIncludePath(b.path("."));
    exe.addIncludePath(b.path("3rd Party"));
    exe.addIncludePath(.{ .cwd_relative = "D:\\3p\\xerces-c-3.3.0\\include" });
    exe.addIncludePath(.{ .cwd_relative = "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include" });

    const libs = [_][]const u8{
        // default imports from visual studio, can probably be paired down.
        "kernel32",
        "user32",
        "gdi32",
        "winspool",
        "comdlg32",
        "advapi32",
        "shell32",
        "ole32",
        "oleaut32",
        "uuid",
        "odbc32",
        "odbccp32",

        // Source added imports
        "xerces-c_3",
        "d3dx9",
        "d3d9",
        "dinput8",
        "xinput",
        "dxguid",
    };

    exe.addLibraryPath(.{ .cwd_relative = "D:\\3p\\xerces-c-3.3.0\\out_x64\\lib"});
    exe.addLibraryPath(.{ .cwd_relative = "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64" });
    for (libs) |lib| {
        exe.linkSystemLibrary(lib);
    }

    exe.addCSourceFiles(.{
        .flags = &.{
            "-std=c++14",
            "-Wno-comment",
            "-Wno-new-returns-null",
            "-D__in=",
            "-D__out=",
            "-D__inout=",
            "-D__reserved=",
        },
        .files = &.{
            "src/50AWE.cpp",
            // "src/ABT.cpp",
            "src/Acidic.cpp",
            "src/Actor.cpp",
            "src/ActorSpawnTrigger.cpp",
            "src/AIEntity.cpp",
            "src/AIGroup.cpp",
            "src/AIManager.cpp",
            "src/AINode.cpp",
            "src/AIStateAcidicAttack.cpp",
            "src/AIStateAcidicFollow.cpp",
            "src/AIStateFollowLeader.cpp",
            "src/AIStateGaseousAttack.cpp",
            "src/AIStateGaseousFollow.cpp",
            "src/AIStateMove.cpp",
            "src/AIStatePathFollow.cpp",
            "src/AIStatePathPlan.cpp",
            "src/AIStateQBFollow.cpp",
            "src/AIStateQBMeleeAttack.cpp",
            "src/AIStateQBRangeAttack.cpp",
            "src/AIStateSpawn.cpp",
            "src/AIStateStraightToGoal.cpp",
            "src/AIStateZombieAttack.cpp",
            // "src/AllocateHierarchy.cpp",
            "src/AnimAllocator.cpp",
            "src/AnimatedMesh.cpp",
            // "src/AnimationInstance.cpp",
            // "src/AnimationManager.cpp",
            "src/BaseGameState.cpp",
            "src/BaseResource.cpp",
            "src/BigCluckingBomb.cpp",
            "src/BitmapFont.cpp",
            "src/Camera.cpp",
            "src/Character.cpp",
            "src/Coin.cpp",
            "src/CollisionGeometry.cpp",
            "src/Command.cpp",
            "src/CommandConsole.cpp",
            "src/ConfigFile.cpp",
            "src/ControlsMenu.cpp",
            "src/CreditsMenu.cpp",
            "src/DebugStream.cpp",
            "src/DesertEagle50.cpp",
            "src/DirectInputWrapper.cpp",
            "src/Enemy.cpp",
            "src/FootBall.cpp",
            "src/FreeCamera.cpp",
            "src/Frustum.cpp",
            // "src/Functor.cpp",
            "src/GameKernel.cpp",
            "src/Gaseous.cpp",
            "src/Geometry.cpp",
            "src/Grenade.cpp",
            "src/HealthPack.cpp",
            "src/HighScoreMenu.cpp",
            "src/HolyWaterBalloonCanon.cpp",
            "src/HUD.cpp",
            "src/IceCream.cpp",
            "src/IntroMenu.cpp",
            "src/Light.cpp",
            "src/LoadState.cpp",
            "src/LoggingSystem.cpp",
            "src/Main.cpp",
            "src/MainButtons.cpp",
            "src/Material.cpp",
            "src/Math3D.cpp",
            "src/Menu.cpp",
            "src/MenuButton.cpp",
            "src/MenuManager.cpp",
            "src/MenuState.cpp",
            "src/Mesh.cpp",
            "src/MultiplayerMenu.cpp",
            "src/Object.cpp",
            "src/ObjectiveHUD.cpp",
            "src/ObjectiveTrigger.cpp",
            "src/ObjectManager.cpp",
            "src/Octant.cpp",
            "src/Octree.cpp",
            "src/Options.cpp",
            "src/OptionsMenu.cpp",
            "src/ParticleAttribute.cpp",
            "src/ParticleEffect.cpp",
            "src/ParticleEffectContentHandler.cpp",
            "src/ParticleEmitter.cpp",
            "src/ParticleKeyframe.cpp",
            "src/ParticleManager.cpp",
            "src/ParticleSystem.cpp",
            "src/PhysicsManager.cpp",
            "src/Pistol.cpp",
            "src/PlacementState.cpp",
            "src/Player.cpp",
            "src/PointLight.cpp",
            "src/Potato.cpp",
            "src/PotatoChainGun.cpp",
            "src/ProjectileWeapon.cpp",
            "src/Projector.cpp",
            "src/QBZombie.cpp",
            "src/RayCastWeapon.cpp",
            "src/RenderDevice.cpp",
            "src/RenderGroup.cpp",
            "src/RenderList.cpp",
            "src/RenderQueue.cpp",
            "src/RenderResource.cpp",
            "src/RenderSystem.cpp",
            "src/ResourceManager.cpp",
            "src/Rocket.cpp",
            "src/RocketLauncher.cpp",
            // "src/Shader.cpp",
            // "src/ShaderInclude.cpp",
            // "src/ShaderManager.cpp",
            // "src/ShaderParam.cpp",
            // "src/ShaderVariant.cpp",
            "src/Shotgun.cpp",
            "src/Sound.cpp",
            "src/SoundManager.cpp",
            "src/SpeedPack.cpp",
            "src/SpotLight.cpp",
            "src/StatSystem.cpp",
            "src/StringA.cpp",
            "src/StringW.cpp",
            "src/Texture2D.cpp",
            "src/Timer.cpp",
            "src/TripleBarrelShotgun.cpp",
            "src/VendingMachine.cpp",
            "src/VendingMachineButtons.cpp",
            "src/VendingState.cpp",
            "src/VertexCache.cpp",
            "src/XInputWrapper.cpp",
            "src/Zombie.cpp",
        },
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    // This allows the user to pass arguments to the application in the build
    // command itself, like this: `zig build run -- arg1 arg2 etc`
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // This creates a build step. It will be visible in the `zig build --help` menu,
    // and can be selected like this: `zig build run`
    // This will evaluate the `run` step rather than the default, which is "install".
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
