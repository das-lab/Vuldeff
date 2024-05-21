import * as path from "path";

import * as Debug from "debug";
const debug = Debug("envcfg");


const yargs = require("yargs-parser");

export interface IConfigSection {
	validate(): void;
}

function isSection(value: any): value is IConfigSection {
	return value && (value as IConfigSection).validate !== undefined;
}

export function validate(configuration: any) {
	for (const configProperty in configuration) {
		if (!configuration.hasOwnProperty(configProperty)) {
			continue;
		}

		const value = configuration[configProperty];
		if (isSection(value)) {
			value.validate();
		}
	}
}

export function applyEnvVariables(configuration: any, envVariables: NodeJS.ProcessEnv, envPrefix = "NODE_ENV") {
	for (const envKey in envVariables) {
		if (!envVariables.hasOwnProperty(envKey)
			|| typeof envVariables[envKey] !== "string"
			|| !envKey.startsWith(envPrefix)
			|| envKey.length <= envPrefix.length) {
			continue;
		}

		const configKey = envKey
			.substr(envPrefix.length)
			.replace(/_/g, ".");

		debug(`Found config value from env '${envKey}' to '${configKey}'`);

		setDeepProperty(configuration, configKey, envVariables[envKey]);
	}
}

export function applyConfigFile(configuration: any, configFile: string) {
	debug(`Loading config form ${configFile}`);
	const config = require(configFile);
	debug(config);

	for (const configProperty in configuration) {
		if (!configuration.hasOwnProperty(configProperty)) {
			continue;
		}

		Object.assign(configuration[configProperty], config[configProperty] || {});
	}
}

export function applyCommandArgs(configuration: any, argv: string[]) {
	if (!argv || !argv.length) {
		return;
	}

	argv = argv.slice(2);

	const parsedArgv = yargs(argv);
	const argvKeys = Object.keys(parsedArgv);
	if (!argvKeys.length) {
		return;
	}

	debug("Appling command arguments:", parsedArgv);

	if (parsedArgv.config) {
		const configFile = path.resolve(process.cwd(), parsedArgv.config);
		applyConfigFile(configuration, configFile);
	}

	for (const key in parsedArgv) {
		if (!parsedArgv.hasOwnProperty(key)) {
			continue;
		}

		const configKey = key
			.replace(/_/g, ".");

		debug(`Found config value from cmd args '${key}' to '${configKey}'`);

		setDeepProperty(configuration, configKey, parsedArgv[key]);
	}
}


export function setDeepProperty(obj: any, propertyPath: string, value: any): void {
	const a = splitPath(propertyPath);
	const n = a.length;

	for (let i = 0; i < n - 1; i++) {
		const k = a[i];

		if (!(k in obj)) {
			obj[k] = {};
		}
		obj = obj[k];
	}


	obj[a[n - 1]] = value;
	return;
}

export function getDeepProperty(obj: any, propertyPath: string): any {
	let ret: any = obj;

	const a = splitPath(propertyPath);
	const n = a.length;

	for (let i = 0; i < n; ++i) {
		const k = a[i];
		if (k in ret) {
			ret = ret[k];
		} else {
			return;
		}
	}

	return ret;
}

export function objectsAreEqual(obj1: any, obj2: any, leftOnly: boolean = false) {
	if (typeof(obj1) === "function") {
		throw new Error("Function compare not supported");
	}

	// is primitive
	if (typeof(obj1) !== "object" || obj1 === null || obj1 === undefined) {
		const exactEqual = obj1 === obj2;
		if (exactEqual) {
			return exactEqual;
		}

		if (isIsoDate(obj1) || isIsoDate(obj2)) {
			return Date.parse(obj1) === Date.parse(obj2);
		}

		return false;
	}

	// Loop through properties in object 1
	for (const p in obj1) {
		if (!obj1.hasOwnProperty(p)) {
			continue;
		}

		// Check property exists on both objects
		if (!obj2.hasOwnProperty(p)) {
			return false;
		}

		if (!objectsAreEqual(obj1[p], obj2[p])) {
			return false;
		}
	}

	if (!leftOnly) {
		// Check object 2 for any extra properties
		for (const p in obj2) {
			if (!obj2.hasOwnProperty(p)) {
				continue;
			}

			// Check property exists on both objects
			if (!obj1.hasOwnProperty(p)) {
				return false;
			}
		}
	}

	return true;
}

function splitPath(propertyPath: string): string[] {
	propertyPath = propertyPath.replace(/\[(\w+)\]/g, ".$1"); // convert indexes to properties
	propertyPath = propertyPath.replace(/^\./, "");           // strip a leading dot
	return propertyPath.split(".");
}

function isIsoDate(value: string): boolean {
	// tslint:disable-next-line:max-line-length
	const ISO_REGEX = /^(?:[1-9]\d{3}-(?:(?:0[1-9]|1[0-2])-(?:0[1-9]|1\d|2[0-8])|(?:0[13-9]|1[0-2])-(?:29|30)|(?:0[13578]|1[02])-31)|(?:[1-9]\d(?:0[48]|[2468][048]|[13579][26])|(?:[2468][048]|[13579][26])00)-02-29)T(?:[01]\d|2[0-3]):[0-5]\d:[0-5]\d(?:\.\d{1,9})?(?:Z|[+-][01]\d:[0-5]\d)$/;
	return ISO_REGEX.test(value);
}
