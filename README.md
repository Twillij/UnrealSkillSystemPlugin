# Skill System

Disclaimer: This documentation is currently just a draft.
Anything written here are highly subject to changes at any point until the completion of the project.

## Table of Contents
> 1. [Skill Execution](#skill-execution)
> 2. [Network Lag Compensation](#network-lag-compensation)

<a name="intro"></a>
## Skill Execution
Skills have different phases of execution.

The flow is as follows:
1. The client sends a command to execute a skill to the server.
2. The server checks whether the skill can be executed.
3. The server sends a multicast RPC with instructions on how to handle the check result.
4. If the skill is interrupted or manually cancelled by the client, the server will be notified.
5. The server sends a multicast RPC to handle the termination of skill regardless of the cause.

## Network Lag Compensation
Currently, the plan is to store any skill commands on the skill component on the client.
These commands will have timestamps that will be sent to the server so that
any lag discrepancies could be handled by the server.
