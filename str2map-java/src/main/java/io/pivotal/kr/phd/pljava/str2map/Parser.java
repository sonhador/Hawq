package io.pivotal.kr.phd.pljava.str2map;

public class Parser {
	public static String extract(String body, String key, String keyValPairsDelim, String keyValDelim) {
		String keyAndDelim = key + keyValDelim;
		
		int pairIdx = body.indexOf(keyAndDelim);
		
		if (pairIdx != 0) {
			pairIdx = body.indexOf(keyValPairsDelim + keyAndDelim);
			
			if (pairIdx == -1) {
				return null;
			}
			
			pairIdx += keyValPairsDelim.length();
		}
		
		int valStartIdx = pairIdx + (keyAndDelim).length();
		int valEndIdx = body.indexOf(keyValPairsDelim, valStartIdx);
		
		if (valStartIdx == valEndIdx) {
			return null;
		}
		
		if (valEndIdx == -1) {
			return body.substring(valStartIdx);
		}
		
		return body.substring(valStartIdx, valEndIdx);
	}
}
