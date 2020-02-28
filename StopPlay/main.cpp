#include <dllentry.h>
#include <log.h>

#include <command.h>
#include <Command/Command.h>
#include <Command/CommandRegistry.h>
#include <Command/CommandPermissionLevel.h>
#include <Command/CommandFlag.h>
#include <Command/CommandParameterData.h>
#include <Command/CommandOutput.h>
#include <Command/CommandOrigin.h>
#include <Core/Packet.h>

#include "settings.h"
#include "playerdb.h"

Settings settings;

DEF_LOGGER("StopPlay");
DEFAULT_SETTINGS(settings);

void dllenter() { LOGV("ENTER"); }
void dllexit() {}

class StopSoundPacket : public Packet {
  std::string name;
  bool stopAll;

public:
  StopSoundPacket(std::string const &name) : name(name), stopAll(false) {}
  StopSoundPacket() : stopAll(true) {}

  inline ~StopSoundPacket() {}
  __declspec(dllimport) virtual MinecraftPacketIds getId() const;
  __declspec(dllimport) virtual std::string getName() const;
  __declspec(dllimport) virtual void write(BinaryStream &) const;
  __declspec(dllimport) virtual PacketReadResult read(ReadOnlyBinaryStream &);
};

class StopPlayCommand : public Command {
public:
  StopPlayCommand() {}

  void execute(CommandOrigin const &origin, CommandOutput &output) {
    if (origin.getOriginType() != CommandOriginType::Player) {
      output.error("commands.generic.error.invalidPlayer", {"/" + settings.CommandName});
      return;
    }
    auto player = (Player *) origin.getEntity();
    StopSoundPacket pkt;
    player->sendNetworkPacket(pkt);
    output.success("commands.stopsound.success.all", {"you"});
  }
  static void setup(CommandRegistry *registry) {
    registry->registerCommand(
        settings.CommandName, "commands.stopsound.description", CommandPermissionLevel::Normal, CommandFlagCheat,
        CommandFlagNone);
    registry->registerOverload<StopPlayCommand>(settings.CommandName);
  }
};

void PostInit() { Mod::CommandSupport::GetInstance().AddListener(SIG("loaded"), &StopPlayCommand::setup); }