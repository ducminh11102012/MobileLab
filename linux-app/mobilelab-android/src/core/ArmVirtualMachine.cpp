#include "ArmVirtualMachine.h"
#include <QStandardPaths>
#include <QFile>
ArmVirtualMachine::ArmVirtualMachine(QObject*p):QObject(p){connect(&m_process,&QProcess::readyReadStandardError,this,[this]{emit logMessage(QString::fromLocal8Bit(m_process.readAllStandardError()));});}
bool ArmVirtualMachine::probe(){m_qemuPath=QStandardPaths::findExecutable("qemu-system-aarch64");m_qemu=!m_qemuPath.isEmpty();m_kvm=QFile::exists("/dev/kvm");emit logMessage(QString("ARM VM backend: QEMU=%1 KVM=%2").arg(m_qemu?"yes":"no",m_kvm?"yes":"no"));return m_qemu;}
bool ArmVirtualMachine::start(const VmSpec&s){if(!m_qemu||running())return false;if(s.disk.isEmpty()&&s.kernel.isEmpty())return false;QStringList a={"-machine","virt","-cpu",m_kvm&&s.acceleration?"host":"max","-smp",QString::number(s.cpus),"-m",QString::number(s.memoryMb)};if(m_kvm&&s.acceleration)a<<"-enable-kvm";if(!s.disk.isEmpty())a<<"-drive"<<"file="+s.disk+",if=virtio,format=qcow2";if(!s.kernel.isEmpty())a<<"-kernel"<<s.kernel;if(!s.initrd.isEmpty())a<<"-initrd"<<s.initrd;a<<"-nographic";m_process.start(m_qemuPath,a);emit logMessage(QString("Started VM %1 using %2").arg(s.name,m_kvm&&s.acceleration?"KVM":"QEMU TCG userspace acceleration"));return m_process.waitForStarted(1500);}
void ArmVirtualMachine::stop(){if(running()){m_process.terminate();if(!m_process.waitForFinished(1000))m_process.kill();emit logMessage("ARM VM stopped");}}
QJsonObject ArmVirtualMachine::status()const{return {{"qemu",m_qemu},{"kvm",m_kvm},{"running",running()},{"mode",m_kvm?"kvm":"tcg"}};}
