import lldb
import re
import testutils as test


def runScenario(assembly, debugger, target):
    process = target.GetProcess()
    res = lldb.SBCommandReturnObject()
    ci = debugger.GetCommandInterpreter()

    # Run debugger, wait until libcoreclr is loaded,
    # set breakpoint at Test.Main and stop there
    test.stop_in_main(debugger, assembly)

    ci.HandleCommand("name2ee " + assembly + " Test.Main", res)
    print(res.GetOutput())
    print(res.GetError())
    # Interpreter must have this command and able to run it
    test.assertTrue(res.Succeeded())

    output = res.GetOutput()
    # Output is not empty
    test.assertTrue(len(output) > 0)

    match = re.search('JITTED Code Address:\s+([0-9a-fA-F]+)', output)
    # Line matched
    test.assertTrue(match)

    groups = match.groups()
    # Match has a single subgroup
    test.assertEqual(len(groups), 1)

    jit_addr = groups[0]
    # Address must be a hex number
    test.assertTrue(test.is_hexnum(jit_addr))

    ci.HandleCommand("ip2md " + jit_addr, res)
    print(res.GetOutput())
    print(res.GetError())
    # Interpreter must have this command and able to run it
    test.assertTrue(res.Succeeded())

    output = res.GetOutput()
    # Output is not empty
    test.assertTrue(len(output) > 0)

    # Specific string must be in the output
    test.assertNotEqual(output.find("MethodDesc:"), -1)

    # TODO: test other use cases

    # Continue current process and checks its exit code
    test.exit_lldb(debugger, assembly)
