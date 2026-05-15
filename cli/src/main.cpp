#include "../include/Include.hpp"

int main(int argc, char** argv)
{
	CLI::App app("Booker - Convert documents between CBZ, CBR and PDF");
	
	std::filesystem::path inputPath;
	std::filesystem::path outputPath;
	bool verbose = false;
	
	app.add_option("input", inputPath, "Input file (.cbz, .cbr, .pdf)")
	  ->required()
	  ->check(CLI::ExistingFile);
	
	app.add_option("output", outputPath, "Output file (.cbz, .cbr, .pdf)")
	  ->required();
	
	app.add_flag("-v,--verbose", verbose, "Verbose output");
	
	try
	{
		CLI11_PARSE(app, argc, argv);
	}
	catch(CLI::ParseError& err)
	{
		return app.exit(err);
	}
	
	auto reader = getReader(inputPath);
	
	if(!reader)
	{
		std::cerr << "ERROR : Unsupported input format: " << inputPath.extension().string() << std::endl;
		
		return 1;
	}
	
	auto writer = getWriter(outputPath);
	
	if(!writer)
	{
		std::cerr << "ERROR : Unsupported output format: " << outputPath.extension().string() << std::endl;
		
		return 1;
	}
	
	if(verbose)
	{
		std::cout << "=== Conversion ===" << std::endl
				  << "Input: " << inputPath.string() << std::endl
				  << "Output: " << outputPath.string() << std::endl
				  << "==================" << std::endl;
	}
	
	std::cout << "Reading: " << inputPath.filename().string() << std::endl;
	
	std::unique_ptr<booker::Document> doc = booker::Document::open(inputPath, reader);
	
	if(!doc)
	{
		std::cerr << "ERROR: Failed to open input file." << std::endl;
		
		return 1;
	}
	
	if(verbose)
	{
		std::cout << "Pages: " << doc->pageCount() << std::endl;
		std::cout << "Title: " << doc->metadata().title() << std::endl;
	}
	
	std::cout << "Converting to: " << outputPath.filename().string() << std::endl;
	
	bool success = writer->writeDocument(*doc, outputPath);
	
	if(!success)
	{
		std::cerr << "ERROR: Failed to write output file" << std::endl;
		
		return 1;
	}
	
	std::cout << "Done!" << std::endl;
	
	return 0;
}